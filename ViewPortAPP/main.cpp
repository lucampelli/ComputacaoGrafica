/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Grupo10 - Luca Campelli e Jonatan Guesser
 *
 * Created on 8 de Março de 2017, 14:03
 */

#include <gtk/gtk.h>
#include <cstdlib>
#include <string.h>
#include "ListaEnc.hpp"
#include "Camera3D.hpp"
#include "Shape.hpp"
#include "Matriz.hpp"

using namespace std;

static cairo_surface_t * surface = NULL;
static GtkWidget *main_window;
static GtkWidget *trans_window;
static GtkWidget *da;
static GtkWidget *shapeField;
static GtkWidget *combo_box_shape;
static GtkWidget *zoomField;
static GtkWidget *buttonAdd;

static GtkWidget *CXE1, *CXE2, *CYE1, *CYE2;
static GtkWidget *entryX, *entryY, *entryS, *entryA;

ListaEnc<Shape*> * lista;
int shape_choice = 0;
int delete_choice = 0;
int transform_choice = 0;
int clicks = 0;
int rectangles_created = 0;
int squares_created = 0;
int polygons_created = 0;
float TX = 0;
float TY = 0;
int rotation = 0;
float SX = 1;
float SY = 1;
bool clicking = false;
ListaEnc<Ponto*> * polP;
Camera3D* cam = Camera3D::getInstance();
Ponto* camPos = cam->getPos();

static void clear_surface() {
    cairo_t * cr;
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    cairo_destroy(cr);
}

static gboolean create_surface(GtkWidget *widget, GdkEventConfigure *event, gpointer data) {
    if (surface)
        cairo_surface_destroy(surface);

    surface = gdk_window_create_similar_surface(gtk_widget_get_window(widget),
            CAIRO_CONTENT_COLOR,
            gtk_widget_get_allocated_width(widget),
            gtk_widget_get_allocated_height(widget));
    clear_surface();
    return TRUE;
}


//OnDrawEvent happens every time that is issued a draw command to the drawing area

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);

    string print = "";

    for (int i = 0; i < lista->getSize(); i++) {
        print.append(lista->get(i)->getName());
        print.append("\n");
        lista->get(i)->draw(cr);
    }

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(shapeField));
    gtk_text_buffer_set_text(buffer, print.c_str(), -1);
    return FALSE;
}

static void remove() {
    lista->retiraDaPosicao(delete_choice);
    gtk_widget_queue_draw(main_window); //good
}

static void on_changed(GtkComboBox *widget, gpointer user_data) {
    GtkComboBox *combo_box = widget;

    if (gtk_combo_box_get_active(combo_box) != 0) {
        shape_choice = gtk_combo_box_get_active(combo_box); //good
    }
}

static void on_changed_trans(GtkComboBox *widget, gpointer user_data) {
    GtkComboBox *combo_box = widget;
    transform_choice = gtk_combo_box_get_active(combo_box);
}

static void on_changed_del(GtkComboBox *widget, gpointer user_data) {
    GtkComboBox *combo_box = widget;
    delete_choice = gtk_combo_box_get_active(combo_box);
}

static void delete_shape() {
    GtkWidget *window;
    GtkWidget *label;
    GtkWidget *combo_box;
    GtkWidget *delB, *cancelB;
    GtkWidget *vbox, *hbox, *hbox2;

    delete_choice = 0;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);

    label = gtk_label_new("Index:");
    combo_box = gtk_combo_box_text_new();
    delB = gtk_button_new_with_label("Remover");
    cancelB = gtk_button_new_with_label("Cancela");



    for (int i = 0; i < lista->getSize(); i++) {
        const gchar* c = lista->get(i)->getName().c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), c);
    }


    g_signal_connect(cancelB, "clicked", G_CALLBACK(gtk_window_close), window); //decent
    g_signal_connect(combo_box, "changed", G_CALLBACK(on_changed_del), NULL);
    g_signal_connect(delB, "clicked", G_CALLBACK(remove), NULL);
    g_signal_connect(delB, "clicked", G_CALLBACK(gtk_window_close), window);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box), 0);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_container_add(GTK_CONTAINER(hbox), label);
    gtk_container_add(GTK_CONTAINER(hbox), combo_box);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_container_add(GTK_CONTAINER(vbox), hbox2);
    gtk_container_add(GTK_CONTAINER(hbox2), delB);
    gtk_container_add(GTK_CONTAINER(hbox2), cancelB);

    gtk_widget_show_all(window);
}

static void zoomIn() {

    cam->Zoom(true);
    gtk_entry_set_text(GTK_ENTRY(zoomField), g_strdup_printf("%.0f", cam->getZoom() * 100)); //ok
    gtk_widget_queue_draw(main_window);
}

static void zoomOut() {

    cam->Zoom(false);
    gtk_entry_set_text(GTK_ENTRY(zoomField), g_strdup_printf("%.0f", cam->getZoom() * 100)); //ok
    gtk_widget_queue_draw(main_window);
}

static void move_shape(Shape* s, float Dx, float Dy) {
    Transform* t = Transform::getInstance();
    t->setT(t->set_2D_move_matrix(Dx, Dy));

    s->move(Dx, Dy);
}

static void scale_shape(Shape* s, float scaleX, float scaleY, Ponto* scale_center = new Ponto(0, 0)) {
    Transform* t = Transform::getInstance();
    t->setT(t->set_2D_move_matrix(-scale_center->getX(), -scale_center->getY()));
    t->concatenate_matrix(t->set_2D_scale_matrix(scaleX, scaleY));
    t->concatenate_matrix(t->set_2D_move_matrix(scale_center->getX(), scale_center->getY()));
    if (scale_center->getX() == s->findCenter()->getX() && scale_center->getY() == s->findCenter()->getY()) {
        s->setScale(scaleX, scaleY);
    } else {
        s->applyT();
    }

}

static void rotate_shape(Shape* s, int degrees, Ponto* rot_center = new Ponto(0, 0)) {
    Transform* t = Transform::getInstance();
    t->setT(t->set_2D_move_matrix(-rot_center->getX(), -rot_center->getY()));
    t->concatenate_matrix(t->set_2D_rotation_matrix(degrees));
    t->concatenate_matrix(t->set_2D_move_matrix(rot_center->getX(), rot_center->getY()));
    if (rot_center->getX() == s->findCenter()->getX() && rot_center->getY() == s->findCenter()->getY()) {
        s->setRot(degrees);
    } else {
        s->applyT();
    }
}

/*
 Rodar a camera significa girar todas as formas ao redor da posicao da camera
 */
static void rotate_cam(int degrees) {
    cam->rotateCamera(degrees);
    for (int i = 0; i < lista->getSize(); i++) {
        rotate_shape(lista->get(i), degrees, cam->getPos());
    }
}

/*
 roda a camera em 10 graus
 */
static void cam_cw() {
    rotate_cam(-10);
    gtk_widget_queue_draw(main_window);
}

static void cam_ccw() {
    rotate_cam(10);
    gtk_widget_queue_draw(main_window);
}


/*
 Arruma as variáveis para rodar uma forma
 */

static void rotate() {    
    Shape* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(CXE2));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(CYE2));

    cout << x1 << ", " << y1 << endl;
    
    if (strlen(x1) == 0) {
        TX = s->findCenter()->getX();
    } else {
        TX = atoi(x1);
    }
    if (strlen(y1) == 0) {
        TY = s->findCenter()->getY();
    } else {
        TY = atoi(y1);
    }
    if (gtk_entry_get_text(GTK_ENTRY(entryA)) == "") {
        rotation = 0;
    } else {
        rotation = atoi(gtk_entry_get_text(GTK_ENTRY(entryA)));
    }

    cout << TX << ", " << TY << endl;

    rotate_shape(s, rotation, new Ponto(TX, TY));
}


/*
 Arruma as variáveis para aumentar uma forma
 */
static void scale() {
    Shape* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(CXE1));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(CYE1));

    if (strlen(x1) == 0) {
        TX = s->findCenter()->getX();
    } else {
        TX = atoi(x1);
    }
    if (strlen(y1) == 0) {
        TY = s->findCenter()->getY();
    } else {
        TY = atoi(y1);
    }

    if (gtk_entry_get_text(GTK_ENTRY(entryS)) == "") {
        SX = 1;
        SY = 1;
    } else {
        SX = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
        SY = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
    }

    scale_shape(s, SX, SY, new Ponto(TX, TY));
}

/*
 Arruma as variáveis para reduzir uma forma
 */
static void unscale(){
    Shape* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(CXE1));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(CYE1));

    if (strlen(x1) == 0) {
        TX = s->findCenter()->getX();
    } else {
        TX = atoi(x1);
    }
    if (strlen(y1) == 0) {
        TY = s->findCenter()->getY();
    } else {
        TY = atoi(y1);
    }

    if (gtk_entry_get_text(GTK_ENTRY(entryS)) == "") {
        SX = 1;
        SY = 1;
    } else {
        SX = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
        SY = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
    }

    scale_shape(s, 1/SX, 1/SY, new Ponto(TX, TY));
}


/*
 Arruma as variáveis para mover uma forma
 */
static void move() {
    Shape* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(entryX));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(entryY));

    if (strlen(x1) == 0) {
        TX = 0;
    } else {
        TX = atoi(x1);
    }
    if (strlen(y1) == 0) {
        TY = 0;
    } else {
        TY = atoi(y1);
    }
    move_shape(s, TX, TY);
}
/*
 Abre a caixa de dialogo de popup
 */
static void transform_shape() {


    GtkWidget *button_move;
    GtkWidget *button_expand;
    GtkWidget *button_reduce;
    GtkWidget *button_rotate;
    GtkWidget *combobox_translate, *combobox_scale, *combobox_rotation;
    GtkWidget *vbox1, *vbox2, *vbox3, *note;
    GtkWidget *hbox1, *hbox2, *hbox3, *hbox21, *hbox31, *hbox22;
    GtkWidget *label1, *label2, *label3;
    GtkWidget *XLabel, *YLabel, *ScaleLabel, *AngleLabel;
    GtkWidget *CXL1, *CXL2, *CYL1, *CYL2;


    transform_choice = 0;

    //gtkBuilder = gtk_builder_new();


    trans_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    entryX = gtk_entry_new();
    entryY = gtk_entry_new();
    entryS = gtk_entry_new();
    entryA = gtk_entry_new();

    CXL1 = gtk_label_new("X: ");
    CXL2 = gtk_label_new("X: ");
    CYL1 = gtk_label_new("Y: ");
    CYL2 = gtk_label_new("Y: ");
    XLabel = gtk_label_new("X: ");
    YLabel = gtk_label_new("Y: ");
    ScaleLabel = gtk_label_new("Multiplicador: ");
    AngleLabel = gtk_label_new("Angulo: ");
    label1 = gtk_label_new("Mover");
    label2 = gtk_label_new("Escalar");
    label3 = gtk_label_new("Rotacionar");

    CXE1 = gtk_entry_new();
    CYE1 = gtk_entry_new();
    CXE2 = gtk_entry_new();
    CYE2 = gtk_entry_new();

    button_move = gtk_button_new_with_label("Mover");
    button_expand = gtk_button_new_with_label("Expandir");
    button_reduce = gtk_button_new_with_label("Reduzir");
    button_rotate = gtk_button_new_with_label("Rotacionar");

    note = gtk_notebook_new();

    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    vbox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    hbox3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    hbox21 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    hbox22 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    hbox31 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    combobox_translate = gtk_combo_box_text_new();
    combobox_scale = gtk_combo_box_text_new();
    combobox_rotation = gtk_combo_box_text_new();

    gtk_window_set_position(GTK_WINDOW(trans_window), GTK_WIN_POS_CENTER);

    gtk_container_add(GTK_CONTAINER(vbox1), combobox_translate);
    gtk_container_add(GTK_CONTAINER(vbox1), hbox1);
    gtk_container_add(GTK_CONTAINER(hbox1), XLabel);
    gtk_container_add(GTK_CONTAINER(hbox1), entryX);
    gtk_container_add(GTK_CONTAINER(hbox1), YLabel);
    gtk_container_add(GTK_CONTAINER(hbox1), entryY);
    gtk_container_add(GTK_CONTAINER(vbox1), button_move);

    gtk_container_add(GTK_CONTAINER(vbox2), combobox_scale);
    gtk_container_add(GTK_CONTAINER(vbox2), hbox2);
    gtk_container_add(GTK_CONTAINER(vbox2), hbox21);
    gtk_container_add(GTK_CONTAINER(vbox2), hbox22);
    gtk_container_add(GTK_CONTAINER(hbox21), CXL1);
    gtk_container_add(GTK_CONTAINER(hbox21), CXE1);
    gtk_container_add(GTK_CONTAINER(hbox21), CYL1);
    gtk_container_add(GTK_CONTAINER(hbox21), CYE1);
    gtk_container_add(GTK_CONTAINER(hbox2), ScaleLabel);
    gtk_container_add(GTK_CONTAINER(hbox2), entryS);
    gtk_container_add(GTK_CONTAINER(hbox22), button_expand);
    gtk_container_add(GTK_CONTAINER(hbox22), button_reduce);

    gtk_container_add(GTK_CONTAINER(vbox3), combobox_rotation);
    gtk_container_add(GTK_CONTAINER(vbox3), hbox3);
    gtk_container_add(GTK_CONTAINER(vbox3), hbox31);
    gtk_container_add(GTK_CONTAINER(hbox3), AngleLabel);
    gtk_container_add(GTK_CONTAINER(hbox31), CXL2);
    gtk_container_add(GTK_CONTAINER(hbox31), CXE2);
    gtk_container_add(GTK_CONTAINER(hbox31), CYL2);
    gtk_container_add(GTK_CONTAINER(hbox31), CYE2);
    gtk_container_add(GTK_CONTAINER(hbox3), entryA);
    gtk_container_add(GTK_CONTAINER(vbox3), button_rotate);

    gtk_notebook_append_page(GTK_NOTEBOOK(note), vbox1, label1);
    gtk_notebook_append_page(GTK_NOTEBOOK(note), vbox2, label2);
    gtk_notebook_append_page(GTK_NOTEBOOK(note), vbox3, label3);

    gtk_container_add(GTK_CONTAINER(trans_window), note);

    for (int i = 0; i < lista->getSize(); i++) {
        const gchar* c = lista->get(i)->getName().c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_translate), c);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_scale), c);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_rotation), c);
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_translate), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_scale), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_rotation), 0);




    g_signal_connect(button_move, "clicked", G_CALLBACK(move), NULL);
    g_signal_connect(button_expand, "clicked", G_CALLBACK(scale), NULL);
    g_signal_connect(button_reduce, "clicked", G_CALLBACK(unscale), NULL);
    g_signal_connect(button_rotate, "clicked", G_CALLBACK(rotate), NULL);
    g_signal_connect(button_move, "clicked", G_CALLBACK(gtk_window_close), NULL);
    g_signal_connect(button_expand, "clicked", G_CALLBACK(gtk_window_close), NULL);
    g_signal_connect(button_reduce, "clicked", G_CALLBACK(gtk_window_close), NULL);
    g_signal_connect(button_rotate, "clicked", G_CALLBACK(gtk_window_close), NULL);


    g_signal_connect(combobox_translate, "changed", G_CALLBACK(on_changed_trans), NULL);
    g_signal_connect(combobox_rotation, "changed", G_CALLBACK(on_changed_trans), NULL);
    g_signal_connect(combobox_scale, "changed", G_CALLBACK(on_changed_trans), NULL);

    if(lista->getSize() == 0){
        
        gtk_widget_set_sensitive(button_move, FALSE);
        gtk_widget_set_sensitive(button_expand, FALSE);
        gtk_widget_set_sensitive(button_reduce, FALSE);
        gtk_widget_set_sensitive(button_rotate, FALSE);
    }
    
    gtk_widget_show_all(trans_window);

}

/*
static void transform_shape(){
    
    GtkBuilder  *gtkBuilder;
    GtkWidget *entryX, *entryY;
    GtkWidget *button_move;
    GtkWidget *button_expand, *button_decrease;
    GtkWidget *button_antirotate, *button_rotate;
    GtkWidget *combobox_translate, *combobox_scale, *combobox_rotation;
    
    transform_choice = 0;

    //gtkBuilder = gtk_builder_new();
    gtkBuilder = gtk_builder_new_from_file("ViewportApp1.glade");
    
    
    trans_window = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "transform_window") );
    entryX = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "entry_X") );
    entryY = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "entry_Y") );
    button_move = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_trans") );
    button_expand = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_plus") );
    button_decrease = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_minus") );
    button_antirotate = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_antiRot") );
    button_rotate = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "button_Rot") );
    //combobox_translate = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "combobox_select") );
    //combobox_scale = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "combobox_select2") );
    //combobox_rotation = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(gtkBuilder), "combobox_select3") );
    
    combobox_translate = gtk_combo_box_text_new();
    combobox_scale = gtk_combo_box_text_new();
    combobox_rotation = gtk_combo_box_text_new();
    
    gtk_window_set_position(GTK_WINDOW(trans_window),GTK_WIN_POS_CENTER);

    
    for (int i = 0; i < lista->getSize(); i++) {
        const gchar* c = lista->get(i)->getName().c_str();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_translate), c);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_scale), c);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_rotation), c);
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_translate),0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_scale),0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox_rotation),0);
    
        
    /*
    g_signal_connect(button_move, "clicked", G_CALLBACK(moveShape), NULL);
    g_signal_connect(button_expand, "clicked", G_CALLBACK(scaleShape), NULL);
    g_signal_connect(button_decrease, "clicked", G_CALLBACK(scaleShape), NULL);
    g_signal_connect(button_antirotate, "clicked", G_CALLBACK(rotateShape), NULL);
    g_signal_connect(button_rotate, "clicked", G_CALLBACK(rotateShape), NULL);
     
    
    g_signal_connect(combobox_translate,"changed",G_CALLBACK(on_changed_trans),NULL);
    g_signal_connect(combobox_rotation,"changed",G_CALLBACK(on_changed_trans),NULL);
    g_signal_connect(combobox_scale,"changed",G_CALLBACK(on_changed_trans),NULL);
    
    gtk_widget_show_all(trans_window);

}
 */
static void build_shape() {

    if (shape_choice == 0) {
        return;
    }
    if (!clicking) {
        clicking = true;
        gtk_widget_set_sensitive(combo_box_shape, FALSE);
        if (shape_choice != 3) {
            gtk_widget_set_sensitive(buttonAdd, FALSE);
        }
        polP = new ListaEnc<Ponto*>();
        return;
    }

    if (shape_choice == 1) {
        //Retangulo
        if (clicking) {
            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            gtk_widget_set_sensitive(buttonAdd, TRUE);
            float sizex = polP->get(1)->getX() - polP->get(0)->getX();
            float sizey = polP->get(1)->getY() - polP->get(0)->getY();
            Retangulo* r = new Retangulo(polP->getHead()->getX() - camPos->getX(),
                    polP->getHead()->getY() - camPos->getY(),
                    sizex, sizey);
            rectangles_created++;
            string new_name = "Retângulo " + std::to_string(rectangles_created);
            r->setName(new_name);
            lista->adiciona(r);
        }

    }
    if (shape_choice == 2) {
        //Quadrado
        if (clicking) {
            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            gtk_widget_set_sensitive(buttonAdd, TRUE);
            float sizex = polP->get(1)->getX() - polP->get(0)->getX();
            float sizey = polP->get(1)->getY() - polP->get(0)->getY();
            Quadrado* q = new Quadrado(polP->getHead()->getX() - camPos->getX(),
                    polP->getHead()->getY() - camPos->getY(),
                    sizex > sizey ? sizex : sizey); //poligon points list = good
            squares_created++;
            string new_name = "Quadrado " + std::to_string(squares_created);
            q->setName(new_name);
            lista->adiciona(q);
        }

    }

    if (shape_choice == 3) {
        //Polígono
        if (clicking) {

            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            Poligono* p = new Poligono(-camPos->getX(), -camPos->getY(), polP);
            polygons_created++;
            string new_name = "Polígono " + std::to_string(polygons_created);
            p->setName(new_name);
            lista->adiciona(p);
        }
    }
    gtk_widget_queue_draw(main_window);
}

static gboolean click(GtkWidget *event_box, GdkEventButton *event, gpointer data) {
    if (!clicking) {
        return FALSE;
    }

    g_print("Event box clicked at coordinates %f : %f\n",
            event->x, event->y);

    polP->adiciona(cam->clickTransform(new Ponto((float) event->x, (float) event->y))); //good
    // Returning TRUE means we handled the event, so the signal
    // emission should be stopped (don’t call any further callbacks
    // that may be connected). Return FALSE to continue invoking callbacks.

    if (shape_choice == 1 || shape_choice == 2) {
        if (polP->getSize() == 2) {
            build_shape();
        }
    }

    return TRUE;

}

void cameraMoveD() {

    cam->moveCamera(0, -10);
    gtk_widget_queue_draw(main_window);
}

void cameraMoveU() {

    cam->moveCamera(0, 10);
    gtk_widget_queue_draw(main_window);
}

void cameraMoveR() {

    cam->moveCamera(-10, 0);
    gtk_widget_queue_draw(main_window);
}

void cameraMoveL() {

    cam->moveCamera(10, 0);
    gtk_widget_queue_draw(main_window);
}

/*
 * 
 */
int main(int argc, char** argv) {

    lista = new ListaEnc<Shape*>();
    //lista->getHead()->printPontos();

    GtkWidget *buttonUp;
    GtkWidget *buttonDown;
    GtkWidget *buttonLeft;
    GtkWidget *buttonRight;
    GtkWidget *buttonRotcw;
    GtkWidget *buttonRotccw;

    GtkWidget *buttonZ1;
    GtkWidget *buttonZ2;
    GtkWidget *delete_button;
    GtkWidget *event_box;

    GtkWidget *hbox, *vbox1, *vbox2, *hboxlr, *hboxZ, *vboxZ, *hboxRot;
    GtkWidget *camLabel, *camRLabel, *zoomLabel;

    GtkWidget *buttonTrans;

    gtk_init(&argc, &argv);

    //Main Window
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(main_window), "Computação Grafica");
    gtk_window_set_position(GTK_WINDOW(main_window), GtkWindowPosition(GTK_WIN_POS_CENTER));
    gtk_widget_set_size_request(main_window, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 11);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //ComboBox
    combo_box_shape = gtk_combo_box_text_new();
    const char* shapes[] = {"Selecione Forma", "Retangulo", "Quadrado", "Poligono"};

    for (int i = 0; i < G_N_ELEMENTS(shapes); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box_shape), shapes[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_box_shape), 0);

    //Containers
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    hboxlr = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    hboxZ = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    vboxZ = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    hboxRot = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    //Drawing Area
    da = gtk_drawing_area_new();

    //Buttons
    buttonAdd = gtk_button_new_with_label("Adicionar Forma");
    buttonUp = gtk_button_new_with_label("▲");
    buttonDown = gtk_button_new_with_label("▼");
    buttonRight = gtk_button_new_with_label("►");
    camLabel = gtk_label_new("Camera");
    camRLabel = gtk_label_new("Cam \n Rotacao");
    buttonRotcw = gtk_button_new_with_label("+10");
    buttonRotccw = gtk_button_new_with_label("-10");
    buttonLeft = gtk_button_new_with_label("◄");
    buttonZ1 = gtk_button_new_with_label("+");
    buttonZ2 = gtk_button_new_with_label("-");
    zoomField = gtk_entry_new();
    zoomLabel = gtk_label_new("Zoom: ");
    event_box = gtk_event_box_new();
    delete_button = gtk_button_new_with_label("Remover Forma");
    buttonTrans = gtk_button_new_with_label("Transformar...");

    //TextField
    shapeField = gtk_text_view_new();

    //Add all the buttons!!!!
    gtk_container_add(GTK_CONTAINER(vbox1), shapeField);
    gtk_container_add(GTK_CONTAINER(vbox1), combo_box_shape);
    gtk_container_add(GTK_CONTAINER(vbox1), buttonAdd);
    gtk_container_add(GTK_CONTAINER(vbox1), delete_button);
    gtk_container_add(GTK_CONTAINER(vbox1), buttonUp);
    gtk_container_add(GTK_CONTAINER(vbox1), hboxlr);
    gtk_container_add(GTK_CONTAINER(hboxlr), buttonLeft);
    gtk_container_add(GTK_CONTAINER(hboxlr), camLabel);
    gtk_container_add(GTK_CONTAINER(hboxlr), buttonRight);
    gtk_container_add(GTK_CONTAINER(vbox1), buttonDown);
    gtk_container_add(GTK_CONTAINER(vbox1), hboxZ);

    gtk_container_add(GTK_CONTAINER(vbox1), buttonTrans);

    gtk_container_add(GTK_CONTAINER(hboxZ), zoomLabel);
    gtk_container_add(GTK_CONTAINER(hboxZ), zoomField);
    gtk_container_add(GTK_CONTAINER(hboxZ), vboxZ);
    gtk_container_add(GTK_CONTAINER(vboxZ), buttonZ1);
    gtk_container_add(GTK_CONTAINER(vboxZ), buttonZ2);

    gtk_container_add(GTK_CONTAINER(event_box), da);
    gtk_container_add(GTK_CONTAINER(vbox2), event_box);
    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_container_add(GTK_CONTAINER(hbox), vbox1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox2);
    gtk_container_add(GTK_CONTAINER(vbox1), hboxRot);
    gtk_container_add(GTK_CONTAINER(hboxRot), buttonRotccw);
    gtk_container_add(GTK_CONTAINER(hboxRot), camRLabel);
    gtk_container_add(GTK_CONTAINER(hboxRot), buttonRotcw);
    

    gtk_widget_set_size_request(event_box, 600, 600);
    gtk_widget_set_size_request(da, 600, 600);
    gtk_widget_set_size_request(buttonLeft, 80, 50);
    gtk_widget_set_size_request(camLabel, 80, 50);
    gtk_widget_set_size_request(buttonRight, 80, 50);
    gtk_widget_set_size_request(shapeField, 200, 100);
    gtk_widget_set_size_request(zoomField, 50, 0);

    gtk_text_view_set_editable(GTK_TEXT_VIEW(shapeField), FALSE);

    g_signal_connect(buttonUp, "clicked", G_CALLBACK(cameraMoveU), NULL);
    g_signal_connect(buttonRight, "clicked", G_CALLBACK(cameraMoveR), NULL);
    g_signal_connect(buttonDown, "clicked", G_CALLBACK(cameraMoveD), NULL);
    g_signal_connect(buttonLeft, "clicked", G_CALLBACK(cameraMoveL), NULL);
    g_signal_connect(buttonAdd, "clicked", G_CALLBACK(build_shape), NULL);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_shape), NULL);
    g_signal_connect(da, "configure-event", G_CALLBACK(create_surface), NULL);
    g_signal_connect(da, "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(G_OBJECT(event_box), "button-press-event", G_CALLBACK(click), da);
    g_signal_connect(combo_box_shape, "changed", G_CALLBACK(on_changed), NULL);
    g_signal_connect(buttonZ1, "clicked", G_CALLBACK(zoomIn), NULL);
    g_signal_connect(buttonZ2, "clicked", G_CALLBACK(zoomOut), NULL);
    g_signal_connect(buttonRotccw, "clicked", G_CALLBACK(cam_ccw), NULL);
    g_signal_connect(buttonRotcw, "clicked", G_CALLBACK(cam_cw), NULL);

    g_signal_connect(buttonTrans, "clicked", G_CALLBACK(transform_shape), NULL);

    gtk_widget_show_all(main_window);

    gtk_entry_set_text(GTK_ENTRY(zoomField), g_strdup_printf("%.0f", cam->getZoom() * 100));



    gtk_main();


    return 0;
}

