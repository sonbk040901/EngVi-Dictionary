#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/bt-5.0.1/inc/btree.h"
#include "sd.h"
#include <ctype.h>
#define MAX_SIZE 10000
/************************************************ POINTER ************************************************/
GtkWidget *window;                                                                                        //window
GtkWidget *add_button, *update_button, *delete_button, *search_entry;                                     //header_bar
GtkWidget *status_label;                                                                                  //status
GtkWidget *ask_box, *question_label, *no_button, *yes_add_button, *yes_update_button, *yes_delete_button; //ask
GtkWidget *meaning_textview, *title_label;                                                                //meaning
GtkWidget *comple, *liststore;                                                                            //suggest
GObject *textbuffer;
/*********************************************************************************************************/
BTA *BT;
void get_object(GtkBuilder *builder)
{
    //window
    window = gtk_builder_get_object(builder, "window");
    //header_bar
    add_button = gtk_builder_get_object(builder, "add_button");
    update_button = gtk_builder_get_object(builder, "update_button");
    delete_button = gtk_builder_get_object(builder, "delete_button");
    search_entry = gtk_builder_get_object(builder, "search_entry");
    //status
    status_label = gtk_builder_get_object(builder, "status_label");
    //ask
    ask_box = gtk_builder_get_object(builder, "ask_box");
    question_label = gtk_builder_get_object(builder, "question_label");
    no_button = gtk_builder_get_object(builder, "no_button");
    yes_add_button = gtk_builder_get_object(builder, "yes_add_button");
    yes_update_button = gtk_builder_get_object(builder, "yes_update_button");
    yes_delete_button = gtk_builder_get_object(builder, "yes_delete_button");
    //meaning
    title_label = gtk_builder_get_object(builder, "title_label");
    meaning_textview = gtk_builder_get_object(builder, "meaning_textview");
    //**
    textbuffer = (GtkTextBuffer *)gtk_builder_get_object(builder, "textbuffer");
    //suggest list
    comple = gtk_entry_completion_new();
    gtk_entry_completion_set_text_column(comple, 0);
    liststore = gtk_list_store_new(20, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    gtk_entry_completion_set_model(comple, GTK_TREE_MODEL(liststore));
    gtk_entry_set_completion(GTK_ENTRY(search_entry), comple);
}
void set_meaning_textview_text(char *output)
{
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), output, strlen(output));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(meaning_textview), GTK_TEXT_BUFFER(textbuffer));
}
char *find_code(const char *code)
{
    char *suggest, *index;
    suggest = (char *)malloc(sizeof(char) * MAX_SIZE);
    char word[100], mean[MAX_SIZE];
    int *rsize, dsize = MAX_SIZE, i = 0;
    strcpy(suggest, "Do you mean:\n\n");
    btpos(BT, 1);
    while (i < 20 && !bnxtky(BT, word, &dsize))
    {
        index = getcode(word);
        if (!strcmp(index, code))
        {
            strcat(suggest, word);
            strcat(suggest, "\n");
            i++;
        }
        free(index);
    }
    return suggest;
}
void meaning_textview_suggest(char *text)
{
    char *code, *suggest;
    code = getcode(text);
    suggest = find(code, BT);
    set_meaning_textview_text(suggest);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);
    free(code);
    free(suggest);
}
void search_entry_activate(GtkEntry *entry, gpointer none)
{
    gtk_widget_set_visible(GTK_WIDGET(ask_box), FALSE);
    char text[256];
    char output[MAX_SIZE];
    int v;
    strcpy(text, gtk_entry_get_text(search_entry));
    if (btsel(BT, text, output, MAX_SIZE, &v))
    {
        gtk_widget_set_sensitive(GTK_WIDGET(update_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(delete_button), FALSE);
        strcpy(output, "");
        set_meaning_textview_text(output);
        if (strlen(text))
        {
            meaning_textview_suggest(text);
            gtk_widget_set_sensitive(GTK_WIDGET(add_button), TRUE);
            gtk_label_set_label(status_label, "This word is not in the dictionary, you can add it to the dictionary!");
            gtk_widget_set_visible(GTK_LABEL(status_label), TRUE);
        }
        else
        {
            gtk_widget_set_sensitive(GTK_WIDGET(add_button), FALSE);
            gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
        }
    }
    else
    {
        gtk_widget_set_sensitive(GTK_WIDGET(update_button), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(delete_button), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(add_button), FALSE);
        gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
        set_meaning_textview_text(output);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);
    }
}
int search_entry_key_press(GtkEntry *entry, GdkEvent *event, gpointer none)
{
    GdkEventKey *pressed_key = (GdkEventKey *)event;
    char word[256];
    int v, length;
    strcpy(word, gtk_entry_get_text(search_entry));
    bfndky(BT, word, &v);
    if (pressed_key->keyval == GDK_KEY_Delete)
    {
        gtk_entry_set_text(search_entry, "");
    }
    else if (pressed_key->keyval == GDK_KEY_Escape)
    {
        gtk_main_quit();
    }
    else if (pressed_key->keyval == GDK_KEY_Tab)
    {
        gtk_widget_grab_focus(delete_button); // ??
        bnxtky(BT, word, &v);
        gtk_entry_set_text(search_entry, word);
        search_entry_activate(search_entry, NULL);
    }
    else
    {
        length = strlen(word);
        if (pressed_key->keyval != GDK_KEY_BackSpace)
        {
            if ((pressed_key->keyval != 65364) && (pressed_key->keyval != 65362))
                word[length] = pressed_key->keyval;
            word[length + 1] = '\0';
        }
        else
            word[length - 1] = '\0';
        show_completion(word);
    }
    return FALSE;
}
void show_completion(char *word)
{
    GtkTreeIter Iter;

    // delete all rows from liststore
    gtk_list_store_clear(liststore);

    char suggest_word[255];
    int rsize;

    if (!bfndky(BT, word, &rsize))
    {
        gtk_list_store_append(liststore, &Iter);           // add a new row
        gtk_list_store_set(liststore, &Iter, 0, word, -1); // them gia tri word vao cot 0
    }

    int index, number = 18; // so luong tu goi y
    strcpy(suggest_word, word);
    for (index = 0; (index <= number) && (number < 100); index++)
    {
        int value;
        if (!bnxtky(BT, suggest_word, &value))
        { // Tim kiem tu tiep theo cua word trong tu dien
            if (prefix(word, suggest_word))
            {
                gtk_list_store_append(liststore, &Iter);
                gtk_list_store_set(liststore, &Iter, 0, suggest_word, -1);
            }
            else
                number++;
        }
    }
}
int prefix(const char *string1, const char *string2)
{
    if (strstr(string2, string1) == string2)
        return 1;
    return 0;
}
void add_button_clicked(GtkButton *button, gpointer none)
{
    if (strlen(gtk_entry_get_text(search_entry)) == 0)
    {
        gtk_widget_set_visible(GTK_WIDGET(ask_box), FALSE);
        gtk_widget_set_visible(GTK_WIDGET(yes_add_button), FALSE);
        gtk_widget_set_visible(GTK_WIDGET(no_button), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(add_button), FALSE);
        gtk_widget_set_visible(GTK_WIDGET(yes_update_button), FALSE);
        gtk_widget_set_visible(GTK_WIDGET(yes_delete_button), FALSE);
        gtk_widget_set_visible(GTK_WIDGET(status_label), FALSE);
        set_meaning_textview_text("");
        return;
    }
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
    set_meaning_textview_text("");
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), TRUE);
    gtk_label_set_label(GTK_LABEL(question_label), "Do you want to add it to the dictionary?");
    gtk_widget_set_visible(GTK_WIDGET(ask_box), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add_button), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(no_button), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete_button), FALSE);
}
void no_button_clicked(GtkButton *button, gpointer none)
{
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(ask_box), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(add_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(update_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(delete_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(no_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete_button), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);
    // gtk_entry_set_text(search_entry, "");
    set_meaning_textview_text(" ");
}
void update_button_clicked(GtkButton *button, gpointer none)
{
    gtk_label_set_label(GTK_LABEL(question_label), "Do you want to update the meaning of this word?");
    gtk_widget_set_visible(GTK_WIDGET(ask_box), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update_button), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(no_button), TRUE);
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), TRUE);
}
void delete_button_clicked(GtkButton *button, gpointer none)
{
    gtk_label_set_label(GTK_LABEL(question_label), "Do you want to delete it from the dictionary?");
    gtk_widget_set_visible(GTK_WIDGET(ask_box), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_delete_button), TRUE);
    gtk_widget_set_visible(GTK_WIDGET(yes_update_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(no_button), TRUE);
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(meaning_textview), FALSE);
}
void show_message(GtkWidget *parent, GtkMessageType type, GtkButtonsType buttons, gchar *notification, gchar *content)
{
    GtkWidget *message;
    message = gtk_message_dialog_new(GTK_WINDOW(parent),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     type,
                                     buttons,
                                     "%s", notification);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(message), "%s", content);
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
}
char *get_text_from_textbuffer(GtkWidget *buffer)
{
    textbuffer = gtk_text_view_get_buffer(meaning_textview);
    GtkTextIter startIter;
    GtkTextIter endIter;

    gtk_text_buffer_get_start_iter(buffer, &startIter);
    gtk_text_buffer_get_end_iter(buffer, &endIter);
    return (char *)gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer), &startIter, &endIter, FALSE);
}
void add_new_word(char *word, char *mean)
{
    btins(BT, word, mean, MAX_SIZE);

    gtk_widget_set_visible(GTK_WIDGET(ask_box), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(add_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(update_button), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(delete_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(no_button), FALSE);
    gtk_widget_set_visible(GTK_WIDGET(yes_add_button), FALSE);
    gtk_widget_set_visible(GTK_LABEL(status_label), FALSE);
}
void yes_add_button_clicked(GtkButton *button, gpointer none)
{
    char mean[MAX_SIZE];
    strcpy(mean, get_text_from_textbuffer(textbuffer));
    if (strlen(mean) == 0)
    {
        show_message(window, GTK_MESSAGE_ERROR, GTK_BUTTONS_CANCEL, "EMPTY STRING!", "Can't add this word to the dictionary");
    }
    else
    {
        add_new_word(gtk_entry_get_text(search_entry), mean);
        no_button_clicked(NULL, NULL);
        show_message(window, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "SUCCESS", "Has added this word to the dictionary");
    }
}
void yes_update_button_clicked(GtkButton *button, gpointer none)
{
    char word[100], mean[MAX_SIZE];
    int *rsize, dsize = MAX_SIZE;
    btseln(BT, word, mean, dsize, rsize);
    btselp(BT, word, mean, dsize, rsize);
    char *new_mean = get_text_from_textbuffer(textbuffer);
    // printf("%s %s\n", mean, new_mean);
    if (strcmp(mean, new_mean) == 0)
    {
        show_message(window, GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, "WARNING!", "The meaning of this word cannot be changed");
    }
    else
    {
        btupd(BT, word, new_mean, MAX_SIZE);
        no_button_clicked(NULL, NULL);
        show_message(window, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "SUCCESS", "The new meaning of the word has been updated");
    }
}
void yes_delete_button_clicked(GtkButton *button, gpointer none)
{
    btdel(BT, NULL);
    no_button_clicked(NULL, NULL);
    gtk_entry_set_text(search_entry, "");
    show_message(window, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "SUCCESS", "Has deleted this word from the dictionary");
}
void get_signal()
{
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(search_entry, "activate", G_CALLBACK(search_entry_activate), NULL);
    g_signal_connect(search_entry, "key_press_event", G_CALLBACK(search_entry_key_press), NULL);
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_button_clicked), NULL);
    g_signal_connect(update_button, "clicked", G_CALLBACK(update_button_clicked), NULL);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_button_clicked), NULL);
    g_signal_connect(no_button, "clicked", G_CALLBACK(no_button_clicked), NULL);
    g_signal_connect(yes_add_button, "clicked", G_CALLBACK(yes_add_button_clicked), NULL);
    g_signal_connect(yes_update_button, "clicked", G_CALLBACK(yes_update_button_clicked), NULL);
    g_signal_connect(yes_delete_button, "clicked", G_CALLBACK(yes_delete_button_clicked), NULL);
}
void separate_mean(char mean[])
{
    int m = 0;
    int n = 1;
    while (mean[n] != '\0')
    {
        if (mean[n] == '\\' && mean[n + 1] == 'n')
        {
            mean[m++] = '\n';
            n += 2;
        }
        else
        {
            if (m != n)
                mean[m++] = mean[n++];
            else
            {
                n++;
                m++;
            }
        }
    }
    mean[m] = '\0';
}

int convert_text_to_bt(char *file)
{
    FILE *data;
    if ((data = fopen(file, "r")) == NULL)
    {
        printf("Khong mo duoc file\n");
        return 0;
    }

    BTA *convert;
    convert = btcrt("resource.dat", 0, 0); //?

    btdups(convert, FALSE);
    char word[256], mean[10000];
    int line; //?

    while (fscanf(data, "%[^\t]", word) == 1)
    {
        fgets(mean, 10000, data);
        line++;
        separate_mean(mean);
        btins(convert, word, mean, strlen(mean) + 1);
    }

    printf("%d lines.\n", line);
    btcls(convert);
    fclose(data);
    return 1;
}

int file_exists(char *pathname)
{
    FILE *f;

    f = fopen(pathname, "r");
    if (f == NULL)
    {
        return FALSE;
    }
    else
    {
        fclose(f);
    }
    return TRUE;
}

//main
int main(int argc, char const *argv[])
{
    GtkBuilder *builder;
    gtk_init(&argc, &argv);
    btinit();
    if (!file_exists("resource.dat"))
    {
        convert_text_to_bt("AnhViet.txt");
    }
    btdups(BT, FALSE);
    BT = btopn("resource.dat", 0, FALSE);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "win.glade", NULL);
    get_object(builder);
    get_signal();

    gtk_widget_show(window);
    g_object_unref(builder);

    gtk_main();
    btcls(BT);
    return 0;
}
