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
static GtkWidget *window;
static GtkWidget *da;
static GtkWidget *shapeField;
static GtkWidget *combo_box_shape;
static GtkWidget *zoomField;
GtkWidget *buttonAdd;
ListaEnc<Shape*> * lista;
int shape_choice = 0;
int delete_choice = 0;
int transform_choice = = 0;
int clicks = 0;
int rectangles_created = 0;
int squares_created = 0;
int polygons_created = 0;
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
    gtk_widget_queue_draw(window); //good
}

static void on_changed(GtkComboBox *widget, gpointer user_data) {
    GtkComboBox *combo_box = widget;

    if (gtk_combo_box_get_active(combo_box) != 0) {
        shape_choice = gtk_combo_box_get_active(combo_box); //good
    }
}

static void on_changed_trans(GtkComboBox *widget, gpointer user_data){
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
    gtk_widget_queue_draw(window);
}

static void zoomOut() {

    cam->Zoom(false);
    gtk_entry_set_text(GTK_ENTRY(zoomField), g_strdup_printf("%.0f", cam->getZoom() * 100)); //ok
    gtk_widget_queue_draw(window);
}

static void move_shape(Shape* s, float Dx, float Dy){
    Transform* t = Transform::getInstance();
    t->setT(t->set_2D_move_matrix(Dx,Dy));
    
    s->move(Dx,Dy);
} 

static void scale_shape(Shape* s, float scaleX, float scaleY, Ponto* scale_center = new Ponto(0,0)){
    Transform* t = Transform::getInstance();
    t->setT(t->set_2D_move_matrix(-scale_center->getX(),-scale_center->getY()));
    t->concatenate_matrix(t->set_2D_scale_matrix(scaleX, scaleY));
    t->concatenate_matrix(t->set_2D_move_matrix(scale_center->getX(),scale_center->getY()));
    if(scale_center->getX() == s->findCenter()->getX() && scale_center->getY() == s->findCenter()->getY()){
        s->setScale(scaleX, scaleY);
    } else {
        s->applyT();
    }
    
}

static void rotate_shape(Shape* s, int degrees, Ponto* rot_center = new Ponto(0,0)){
    Transform* t = Transform::getInstance();
    t->setT(t->set_2D_move_matrix(-rot_center->getX(),-rot_center->getY()));
    t->concatenate_matrix(t->set_2D_rotation_matrix(degrees));
    t->concatenate_matrix(t->set_2D_move_matrix(rot_center->getX(),rot_center->getY()));
    if(rot_center->getX() == s->findCenter()->getX() && rot_center->getY() == s->findCenter()->getY()){
        s->setRot(degrees);
    } else {
        s->applyT();
    }
}

static void rotate_cam(int degrees){
    cam->rotateCamera(degrees);
    for(int i = 0; i < lista->getSize(); i++){
        rotate_shape(lista->get(i),cam->getRot());
    }
}


static void transform_shape(){
    //here to popup transform shape
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //here to popup transform shape
}

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
            move_shape(r,40,40);
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
    gtk_widget_queue_draw(window);
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
    gtk_widget_queue_draw(window);
}

void cameraMoveU() {

    cam->moveCamera(0, 10);
    gtk_widget_queue_draw(window);
}

void cameraMoveR() {

    cam->moveCamera(-10, 0);
    gtk_widget_queue_draw(window);
}

void cameraMoveL() {

    cam->moveCamera(10, 0);
    gtk_widget_queue_draw(window);
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

    GtkWidget *buttonZ1;
    GtkWidget *buttonZ2;
    GtkWidget *delete_button;
    GtkWidget *event_box;

    GtkWidget *hbox, *vbox1, *vbox2, *hboxlr, *hboxZ, *vboxZ;
    GtkWidget *camLabel, *zoomLabel;

    gtk_init(&argc, &argv);

    //Window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Computação Grafica");
    gtk_window_set_position(GTK_WINDOW(window), GtkWindowPosition(GTK_WIN_POS_CENTER));
    gtk_widget_set_size_request(window, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

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

    //Drawing Area
    da = gtk_drawing_area_new();

    //Buttons
    buttonAdd = gtk_button_new_with_label("Adicionar Forma");
    buttonUp = gtk_button_new_with_label("▲");
    buttonDown = gtk_button_new_with_label("▼");
    buttonRight = gtk_button_new_with_label("►");
    camLabel = gtk_label_new("Camera");
    buttonLeft = gtk_button_new_with_label("◄");
    buttonZ1 = gtk_button_new_with_label("+");
    buttonZ2 = gtk_button_new_with_label("-");
    zoomField = gtk_entry_new();
    zoomLabel = gtk_label_new("Zoom: ");
    event_box = gtk_event_box_new();
    delete_button = gtk_button_new_with_label("Remover Forma");

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

    gtk_container_add(GTK_CONTAINER(hboxZ), zoomLabel);
    gtk_container_add(GTK_CONTAINER(hboxZ), zoomField);
    gtk_container_add(GTK_CONTAINER(hboxZ), vboxZ);
    gtk_container_add(GTK_CONTAINER(vboxZ), buttonZ1);
    gtk_container_add(GTK_CONTAINER(vboxZ), buttonZ2);

    gtk_container_add(GTK_CONTAINER(event_box), da);
    gtk_container_add(GTK_CONTAINER(vbox2), event_box);
    gtk_container_add(GTK_CONTAINER(window), hbox);
    gtk_container_add(GTK_CONTAINER(hbox), vbox1);
    gtk_container_add(GTK_CONTAINER(hbox), vbox2);

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

    gtk_widget_show_all(window);

    gtk_entry_set_text(GTK_ENTRY(zoomField), g_strdup_printf("%.0f", cam->getZoom() * 100));



    gtk_main();

    
    return 0;
}

