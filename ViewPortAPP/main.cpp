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

#include "Matriz.hpp"

using namespace std;

static cairo_surface_t * surface = NULL;
static GtkWidget *main_window;
static GtkWidget *trans_window;
static GtkWidget *surface_window;
static GtkWidget *da;
static GtkWidget *shapeField;
static GtkWidget *combo_box_shape;
static GtkWidget *zoomField;
static GtkWidget *buttonAdd;

static GtkWidget *fillBox, *ortoSlider;

static GtkWidget *CXE1, *CXE2, *CYE1, *CYE2, *CZE1, *CZE2;
static GtkWidget *entryX, *entryY, *entryZ, *entryS, *entryAx, *entryAy, *entryAz;

static GtkWidget *labels[16];
static GtkWidget *entrysX[16];
static GtkWidget *entrysY[16];
static GtkWidget *entrysZ[16];
static GtkWidget *ButtonOk, *ButtonCancel;

//Shape* clipShape;
ListaEnc<Shape3D*> * lista;
ListaEnc<Shape3D*> * listaPersp;
ListaEnc<Shape3D*> * normLista;

int shape_choice = 0;
int delete_choice = 0;
int transform_choice = 0;
int clicks = 0;
int points_created = 0;
int rectangles_created = 0;
int squares_created = 0;
int polygons_created = 0;
int bezier_created = 0;
int spline_created = 0;
int lines_created = 0;
int bezier_surf_created = 0;
double TX = 0;
double TY = 0;
double TZ = 0;
int rotationX = 0;
int rotationY = 0;
int rotationZ = 0;
double SX = 1;
double SY = 1;
double SZ = 1;
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
    cam->perspective(gtk_range_get_value(GTK_RANGE(ortoSlider)) / 10);
    cam->SCN();
    for (int i = 0; i < normLista->getSize(); i++) {
        print.append(lista->get(i)->getName());
        print.append("\n");
        normLista->get(i)->clip(cam->getClipMin(), cam->getClipMax());
        normLista->get(i)->draw(cr, camPos);

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
    shape_choice = gtk_combo_box_get_active(combo_box); //good
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
    gtk_window_set_title(GTK_WINDOW(window), "Delete Shape");
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

static void move_shape(Shape3D* s, double Dx, double Dy, double Dz) {
    Transform* t = Transform::getInstance();
    t->setT3D(t->set_3D_move_matrix(Dx, Dy, Dz));

    s->applyT();
}

static void scale_shape(Shape3D* s, double scaleX, double scaleY, double scaleZ, Ponto* scale_center = new Ponto(0, 0, 0)) {
    Transform* t = Transform::getInstance();
    t->setT3D(t->set_3D_move_matrix(-scale_center->getX(), -scale_center->getY(), -scale_center->getZ()));
    t->concatenate_matrix_3D(t->set_3D_scale_matrix(scaleX, scaleY, scaleZ));
    t->concatenate_matrix_3D(t->set_3D_move_matrix(scale_center->getX(), scale_center->getY(), scale_center->getZ()));
    if (scale_center->getX() == s->findCenter()->getX() && scale_center->getY() == s->findCenter()->getY() && scale_center->getZ() == s->findCenter()->getZ()) {
        s->setScale(scaleX, scaleY, scaleZ);
    } else {
        s->applyT();
    }

}

static void rotate_shape(Shape3D* s, int degreesX, int degreesY, int degreesZ, Ponto* rot_center = new Ponto(0, 0)) {
    Transform* t = Transform::getInstance();
    t->setT3D(t->set_3D_move_matrix(-rot_center->getX(), -rot_center->getY(), -rot_center->getZ()));
    t->concatenate_matrix_3D(t->set_3Dx_rotation_matrix(degreesX));
    t->concatenate_matrix_3D(t->set_3Dy_rotation_matrix(degreesY));
    t->concatenate_matrix_3D(t->set_3Dz_rotation_matrix(degreesZ));
    t->concatenate_matrix_3D(t->set_3D_move_matrix(rot_center->getX(), rot_center->getY(), rot_center->getZ()));


    if (rot_center->getX() == s->findCenter()->getX() && rot_center->getY() == s->findCenter()->getY() && rot_center->getZ() == s->findCenter()->getZ()) {
        s->setRotX(degreesX);
        s->setRotY(degreesY);
        s->setRotZ(degreesZ);

    }

    s->applyT();

}

/*
 Rodar a camera significa girar todas as formas ao redor da posicao da camera
 */
static void rotate_camX(int degrees) {
    cam->rotateCameraX(degrees);
    gtk_widget_queue_draw(main_window);
}

static void rotate_camY(int degrees) {
    cam->rotateCameraY(degrees);
    gtk_widget_queue_draw(main_window);
}

static void rotate_camZ(int degrees) {
    cam->rotateCameraZ(degrees);
    gtk_widget_queue_draw(main_window);
}

/*
 roda a camera em 10 graus
 */
static void cam_cw() {
    rotate_camZ(10);
}

static void cam_ccw() {
    rotate_camZ(-10);
}

static void cam_rt() {
    rotate_camY(10);
}

static void cam_lf() {
    rotate_camY(-10);
}

static void cam_up() {
    rotate_camX(10);
}

static void cam_dn() {
    rotate_camX(-10);
}

/*
 Arruma as variáveis para rodar uma forma
 */

static void rotate() {
    Shape3D* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(CXE2));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(CYE2));
    const char* z1 = gtk_entry_get_text(GTK_ENTRY(CZE2));

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
    if (strlen(z1) == 0) {
        TZ = s->findCenter()->getZ();
    } else {
        TZ = atoi(z1);
    }

    if (strlen(gtk_entry_get_text(GTK_ENTRY(entryAx))) == 0) {
        rotationX = 0;
    } else {
        rotationX = atoi(gtk_entry_get_text(GTK_ENTRY(entryAx)));
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(entryAy))) == 0) {
        rotationY = 0;
    } else {
        rotationY = atoi(gtk_entry_get_text(GTK_ENTRY(entryAy)));
    }
    if (strlen(gtk_entry_get_text(GTK_ENTRY(entryAz))) == 0) {
        rotationZ = 0;
    } else {
        rotationZ = atoi(gtk_entry_get_text(GTK_ENTRY(entryAz)));
    }

    rotate_shape(s, rotationX, rotationY, rotationZ, new Ponto(TX, TY, TZ));
    gtk_widget_queue_draw(main_window);

}

/*
 Arruma as variáveis para aumentar uma forma
 */
static void scale() {
    Shape3D* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(CXE1));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(CYE1));
    const char* z1 = gtk_entry_get_text(GTK_ENTRY(CZE1));

    if (strlen(x1) == 0) {
        TX = s->findCenter()->getX();
    } else {
        TX = atof(x1);
    }
    if (strlen(y1) == 0) {
        TY = s->findCenter()->getY();
    } else {
        TY = atof(y1);
    }
    if (strlen(z1) == 0) {
        TZ = s->findCenter()->getZ();
    } else {
        TZ = atof(z1);
    }

    if (gtk_entry_get_text(GTK_ENTRY(entryS)) == "") {
        SX = 1;
        SY = 1;
        SZ = 1;
    } else {
        SX = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
        SY = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
        SZ = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
    }

    scale_shape(s, SX, SY, SZ, new Ponto(TX, TY, TZ));
    gtk_widget_queue_draw(main_window);
}

/*
 Arruma as variáveis para reduzir uma forma
 */
static void unscale() {
    Shape3D* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(CXE1));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(CYE1));
    const char* z1 = gtk_entry_get_text(GTK_ENTRY(CZE1));

    if (strlen(x1) == 0) {
        TX = s->findCenter()->getX();
    } else {
        TX = atof(x1);
    }
    if (strlen(y1) == 0) {
        TY = s->findCenter()->getY();
    } else {
        TY = atof(y1);
    }
    if (strlen(z1) == 0) {
        TZ = s->findCenter()->getZ();
    } else {
        TZ = atof(z1);
    }


    if (gtk_entry_get_text(GTK_ENTRY(entryS)) == "") {
        SX = 1;
        SY = 1;
        SZ = 1;
    } else {
        SX = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
        SY = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
        SZ = atof(gtk_entry_get_text(GTK_ENTRY(entryS)));
    }

    scale_shape(s, 1.0 / SX, 1.0 / SY, 1.0 / SZ, new Ponto(TX, TY, TZ));
    gtk_widget_queue_draw(main_window);
}

/*
 Arruma as variáveis para mover uma forma
 */
static void move() {
    Shape3D* s = lista->get(transform_choice);
    const char* x1 = gtk_entry_get_text(GTK_ENTRY(entryX));
    const char* y1 = gtk_entry_get_text(GTK_ENTRY(entryY));
    const char* z1 = gtk_entry_get_text(GTK_ENTRY(entryZ));

    if (strlen(x1) == 0) {
        TX = 0;
    } else {
        TX = atof(x1);
    }
    if (strlen(y1) == 0) {
        TY = 0;
    } else {
        TY = atof(y1);
    }
    if (strlen(z1) == 0) {
        TZ = 0;
    } else {
        TZ = atof(z1);
    }
    move_shape(s, TX / 10, TY / 10, TZ / 10);
    gtk_widget_queue_draw(main_window);
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
    GtkWidget *XLabel, *YLabel, *ZLabel, *ScaleLabel, *AngleLabel;
    GtkWidget *CXL1, *CXL2, *CYL1, *CYL2, *CZL1, *CZL2;


    transform_choice = 0;

    //gtkBuilder = gtk_builder_new();


    trans_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(trans_window), "Transform Shape");
    entryX = gtk_entry_new();
    entryY = gtk_entry_new();
    entryZ = gtk_entry_new();
    entryS = gtk_entry_new();
    entryAx = gtk_entry_new();
    entryAy = gtk_entry_new();
    entryAz = gtk_entry_new();

    CXL1 = gtk_label_new("X: ");
    CXL2 = gtk_label_new("X: ");
    CYL1 = gtk_label_new("Y: ");
    CYL2 = gtk_label_new("Y: ");
    CZL1 = gtk_label_new("Z: ");
    CZL2 = gtk_label_new("Z: ");
    XLabel = gtk_label_new("X: ");
    YLabel = gtk_label_new("Y: ");
    ZLabel = gtk_label_new("Z: ");
    ScaleLabel = gtk_label_new("Multiplicador: ");
    AngleLabel = gtk_label_new("Angulo: ");
    label1 = gtk_label_new("Mover");
    label2 = gtk_label_new("Escalar");
    label3 = gtk_label_new("Rotacionar");

    CXE1 = gtk_entry_new();
    CYE1 = gtk_entry_new();
    CZE1 = gtk_entry_new();
    CXE2 = gtk_entry_new();
    CYE2 = gtk_entry_new();
    CZE2 = gtk_entry_new();

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
    gtk_container_add(GTK_CONTAINER(hbox1), ZLabel);
    gtk_container_add(GTK_CONTAINER(hbox1), entryZ);
    gtk_container_add(GTK_CONTAINER(vbox1), button_move);

    gtk_container_add(GTK_CONTAINER(vbox2), combobox_scale);
    gtk_container_add(GTK_CONTAINER(vbox2), hbox2);
    gtk_container_add(GTK_CONTAINER(vbox2), hbox21);
    gtk_container_add(GTK_CONTAINER(vbox2), hbox22);
    gtk_container_add(GTK_CONTAINER(hbox21), CXL1);
    gtk_container_add(GTK_CONTAINER(hbox21), CXE1);
    gtk_container_add(GTK_CONTAINER(hbox21), CYL1);
    gtk_container_add(GTK_CONTAINER(hbox21), CYE1);
    gtk_container_add(GTK_CONTAINER(hbox21), CZL1);
    gtk_container_add(GTK_CONTAINER(hbox21), CZE1);
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
    gtk_container_add(GTK_CONTAINER(hbox31), CZL2);
    gtk_container_add(GTK_CONTAINER(hbox31), CZE2);
    gtk_container_add(GTK_CONTAINER(hbox3), entryAx);
    gtk_container_add(GTK_CONTAINER(hbox3), entryAy);
    gtk_container_add(GTK_CONTAINER(hbox3), entryAz);
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

    if (lista->getSize() == 0) {

        gtk_widget_set_sensitive(button_move, FALSE);
        gtk_widget_set_sensitive(button_expand, FALSE);
        gtk_widget_set_sensitive(button_reduce, FALSE);
        gtk_widget_set_sensitive(button_rotate, FALSE);
    }

    gtk_widget_show_all(trans_window);

}

static void reenable() {
    gtk_widget_set_sensitive(combo_box_shape, TRUE);
    gtk_widget_set_sensitive(buttonAdd, TRUE);
    clicking = false;
}

static void buildBezier3D() {

    ListaEnc<Ponto*>* pontos = new ListaEnc<Ponto*>();


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox))) {
        for (int i = 0; i < 16; i++) {
            pontos->adiciona(new Ponto(atof(gtk_entry_get_text(GTK_ENTRY(entrysX[i]))), atof(gtk_entry_get_text(GTK_ENTRY(entrysY[i]))), atof(gtk_entry_get_text(GTK_ENTRY(entrysZ[i])))));
            //cout<<pontos->get(i)->getX() << " : " << pontos->get(i)->getY() << " : " << pontos->get(i)->getZ() << endl;
        }
    } else {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                pontos->adiciona(new Ponto(-(i/20.0), -(j/20.0), (i+j)/20.0));
            }
        }
    }


    SurfaceBezier* c = new SurfaceBezier(0.1f, pontos);

    string new_name = "BezierSurface " + std::to_string(bezier_surf_created);

    c->setName(new_name);
    c->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
    lista->adiciona(c);
    reenable();
}

static void TelaBezier3D() {

    GtkWidget *vbox, *hbox[16];

    surface_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(surface_window), "Create Surface");

    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            labels[k] = gtk_label_new(g_strdup_printf("Ponto %i x %i", i, j));
            k++;
        }
    }

    for (int i = 0; i < 16; i++) {
        entrysX[i] = gtk_entry_new();
        entrysY[i] = gtk_entry_new();
        entrysZ[i] = gtk_entry_new();
    }

    ButtonOk = gtk_button_new_with_label("OK");
    ButtonCancel = gtk_button_new_with_label("Cancel");

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    for (int i = 0; i < 16; i++) {
        hbox[i] = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    }

    gtk_container_add(GTK_CONTAINER(surface_window), vbox);

    for (int i = 0; i < 16; i++) {
        gtk_container_add(GTK_CONTAINER(vbox), hbox[i]);
        gtk_container_add(GTK_CONTAINER(hbox[i]), labels[i]);
        gtk_container_add(GTK_CONTAINER(hbox[i]), entrysX[i]);
        gtk_container_add(GTK_CONTAINER(hbox[i]), entrysY[i]);
        gtk_container_add(GTK_CONTAINER(hbox[i]), entrysZ[i]);

    }

    gtk_container_add(GTK_CONTAINER(vbox), ButtonOk);
    gtk_container_add(GTK_CONTAINER(vbox), ButtonCancel);

    g_signal_connect(ButtonOk, "clicked", G_CALLBACK(buildBezier3D), NULL);
    g_signal_connect(ButtonOk, "clicked", G_CALLBACK(gtk_window_close), NULL);
    g_signal_connect(ButtonCancel, "clicked", G_CALLBACK(reenable), NULL);
    g_signal_connect(ButtonCancel, "clicked", G_CALLBACK(gtk_window_close), NULL);

    gtk_window_set_position(GTK_WINDOW(surface_window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(surface_window);

}

static void build_shape() {

    if (!clicking) {
        clicking = true;
        gtk_widget_set_sensitive(combo_box_shape, FALSE);
        if (shape_choice != 4 && shape_choice != 5 && shape_choice != 6) {
            gtk_widget_set_sensitive(buttonAdd, FALSE);
        }
        polP = new ListaEnc<Ponto*>();
        if (shape_choice != 7) {
            return;
        }
    }

    if (polP->getSize() == 0 && shape_choice != 7) {
        return;
    }

    if (shape_choice == 0) {
        if (clicking) {
            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            gtk_widget_set_sensitive(buttonAdd, TRUE);
            Point3D* p = new Point3D(polP->getHead()->getX(), polP->getHead()->getY(), 1);
            points_created++;
            string new_name = "Ponto " + std::to_string(points_created);
            p->setName(new_name);
            p->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(p);
        }
    }

    if (shape_choice == 1) {
        if (clicking) {
            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            gtk_widget_set_sensitive(buttonAdd, TRUE);
            Reta3D* p = new Reta3D(polP->get(0), polP->get(1));
            lines_created++;
            string new_name = "Reta " + std::to_string(lines_created);
            p->setName(new_name);
            p->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(p);
        }
    }

    if (shape_choice == 2) {
        //Retangulo
        if (clicking) {
            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            gtk_widget_set_sensitive(buttonAdd, TRUE);
            double sizex = polP->get(1)->getX() - polP->get(0)->getX();
            double sizey = polP->get(1)->getY() - polP->get(0)->getY();
            Retangulo3D* r = new Retangulo3D(polP->getHead()->getX() - camPos->getX(),
                    polP->getHead()->getY() + camPos->getY(), polP->getHead()->getZ() - camPos->getZ(),
                    sizex, sizey);
            rectangles_created++;
            string new_name = "Retângulo " + std::to_string(rectangles_created);
            r->setName(new_name);
            r->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(r);
        }

    }
    if (shape_choice == 3) {
        //Quadrado
        if (clicking) {
            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            gtk_widget_set_sensitive(buttonAdd, TRUE);
            double sizex = polP->get(1)->getX() - polP->get(0)->getX();
            double sizey = polP->get(1)->getY() - polP->get(0)->getY();
            Quadrado3D* q = new Quadrado3D(polP->getHead()->getX() - camPos->getX(),
                    polP->getHead()->getY() + camPos->getY(), polP->getHead()->getZ() - camPos->getZ(),
                    sizex > sizey ? sizex : sizey); //poligon points list = good 
            squares_created++;
            string new_name = "Quadrado " + std::to_string(squares_created);
            q->setName(new_name);
            q->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(q);
        }

    }

    if (shape_choice == 4) {
        //Polígono
        if (clicking) {

            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            Poligono3D* p = new Poligono3D(-camPos->getX(), camPos->getY(), -camPos->getZ(), polP);
            polygons_created++;
            string new_name = "Polígono " + std::to_string(polygons_created);
            p->setName(new_name);
            p->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(p);
        }
    }

    if (shape_choice == 5) {
        //CurvaBezier
        if (clicking) {

            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            CurvaBezier3D* c = new CurvaBezier3D(-camPos->getX(), camPos->getY(), polP);
            bezier_created++;
            string new_name = "Bezier " + std::to_string(bezier_created);
            c->setName(new_name);
            c->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(c);
        }
    }

    if (shape_choice == 6) {
        //BSpline
        if (clicking) {

            clicking = false;
            gtk_widget_set_sensitive(combo_box_shape, TRUE);
            B_Spline3D* c = new B_Spline3D(-camPos->getX(), camPos->getY(), -camPos->getZ(), polP);
            spline_created++;
            string new_name = "B_Spline " + std::to_string(spline_created);
            c->setName(new_name);
            c->setFill(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox)));
            lista->adiciona(c);
        }
    }

    if (shape_choice == 7) {
        //Surface
        clicking = false;
        gtk_widget_set_sensitive(combo_box_shape, TRUE);

        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fillBox))) {
            TelaBezier3D();
        } else {
            buildBezier3D();
        }

        bezier_surf_created++;
    }
    gtk_widget_queue_draw(main_window);
}

static gboolean click(GtkWidget *event_box, GdkEventButton *event, gpointer data) {
    if (!clicking) {
        return FALSE;
    }

    g_print("Event box clicked at coordinates %f : %f\n",
            event->x, event->y);

    polP->adiciona(cam->clickTransform(new Ponto((double) event->x, (double) event->y))); //good
    // Returning TRUE means we handled the event, so the signal
    // emission should be stopped (don’t call any further callbacks
    // that may be connected). Return FALSE to continue invoking callbacks.

    if (shape_choice == 1 || shape_choice == 2 || shape_choice == 3) {
        if (polP->getSize() == 2) {
            build_shape();
            return TRUE;
        }
    }

    if (shape_choice == 0) {

        build_shape();
    }

    return TRUE;

}

void cameraMoveD() {

    cam->moveCamera(0, -0.1f);

    gtk_widget_queue_draw(main_window);
}

void cameraMoveU() {

    cam->moveCamera(0, 0.1f);
    gtk_widget_queue_draw(main_window);
}

void cameraMoveR() {

    cam->moveCamera(-0.1f, 0);
    gtk_widget_queue_draw(main_window);
}

void cameraMoveL() {

    cam->moveCamera(0.1f, 0);
    gtk_widget_queue_draw(main_window);
}

void update_draw() {

    gtk_widget_queue_draw(main_window);
}

/*
 * 
 */
int main(int argc, char** argv) {

    lista = cam->getShapeList();
    normLista = cam->getNormList();
    listaPersp = cam->getShapePerspList();
    //lista->getHead()->printPontos();

    GtkWidget *buttonUp;
    GtkWidget *buttonDown;
    GtkWidget *buttonLeft;
    GtkWidget *buttonRight;
    GtkWidget *buttonRotcw;
    GtkWidget *buttonRotccw;
    GtkWidget *buttonRotDn;
    GtkWidget *buttonRotUp;
    GtkWidget *buttonRotRt;
    GtkWidget *buttonRotLf;

    GtkWidget *buttonZ1;
    GtkWidget *buttonZ2;
    GtkWidget *delete_button;
    GtkWidget *event_box;

    GtkWidget *hbox, *vbox1, *vbox2, *hboxlr, *hboxZ, *vboxZ, *hboxRot, *hboxRot1;
    GtkWidget *camLabel, *camRLabel, *zoomLabel;

    GtkWidget *buttonTrans;

    gtk_init(&argc, &argv);

    //Main Window
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Bethesda Software Game Engine");

    gtk_window_set_title(GTK_WINDOW(main_window), "Computação Grafica");
    gtk_window_set_position(GTK_WINDOW(main_window), GtkWindowPosition(GTK_WIN_POS_CENTER));
    gtk_widget_set_size_request(main_window, 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 11);

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //ComboBox
    combo_box_shape = gtk_combo_box_text_new();
    const char* shapes[] = {"Ponto", "Reta", "Retangulo", "Quadrado", "Poligono", "Bezier", "B-Spline", "SurfaceBezier3D"};

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
    hboxRot1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    //Drawing Area
    da = gtk_drawing_area_new();

    //Buttons
    buttonAdd = gtk_button_new_with_label("Adicionar Forma");
    fillBox = gtk_check_button_new_with_label("Preencher");
    ortoSlider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 10, 1);
    buttonUp = gtk_button_new_with_label("▲");
    buttonDown = gtk_button_new_with_label("▼");
    buttonRight = gtk_button_new_with_label("►");
    buttonLeft = gtk_button_new_with_label("◄");
    camLabel = gtk_label_new("Camera");
    camRLabel = gtk_label_new("Cam \n Rotacao");
    buttonRotcw = gtk_button_new_with_label("+10 )");
    buttonRotccw = gtk_button_new_with_label("( -10");
    buttonRotUp = gtk_button_new_with_label("▲ +10");
    buttonRotDn = gtk_button_new_with_label("▼ -10");
    buttonRotRt = gtk_button_new_with_label("+10 ►");
    buttonRotLf = gtk_button_new_with_label("◄ -10");

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
    gtk_container_add(GTK_CONTAINER(vbox1), fillBox);
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
    gtk_container_add(GTK_CONTAINER(vbox1), hboxRot1);
    gtk_container_add(GTK_CONTAINER(vbox1), ortoSlider);
    gtk_container_add(GTK_CONTAINER(hboxRot), buttonRotccw);
    gtk_container_add(GTK_CONTAINER(hboxRot1), buttonRotUp);
    gtk_container_add(GTK_CONTAINER(hboxRot1), buttonRotLf);
    gtk_container_add(GTK_CONTAINER(hboxRot), camRLabel);
    gtk_container_add(GTK_CONTAINER(hboxRot1), buttonRotRt);
    gtk_container_add(GTK_CONTAINER(hboxRot1), buttonRotDn);
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
    g_signal_connect(buttonRotRt, "clicked", G_CALLBACK(cam_rt), NULL);
    g_signal_connect(buttonRotLf, "clicked", G_CALLBACK(cam_lf), NULL);
    g_signal_connect(buttonRotUp, "clicked", G_CALLBACK(cam_up), NULL);
    g_signal_connect(buttonRotDn, "clicked", G_CALLBACK(cam_dn), NULL);
    g_signal_connect(GTK_RANGE(ortoSlider), "value-changed", G_CALLBACK(update_draw), NULL);

    g_signal_connect(buttonTrans, "clicked", G_CALLBACK(transform_shape), NULL);

    gtk_widget_show_all(main_window);

    gtk_entry_set_text(GTK_ENTRY(zoomField), g_strdup_printf("%.0f", cam->getZoom() * 100));



    gtk_main();


    return 0;
}

