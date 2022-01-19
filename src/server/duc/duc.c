#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <json-c/json.h>
#include <mysql/mysql.h>

const char *server = "localhost";
const char *user = "root";
const char *password = "root";
const char *database = "youtok";

const char *no_login_all_videos();
const char *login_all_videos(const char *user_id);
const char *current_user_all_videos(const char *user_id);
int check_permission_to_watch_video(const char *user_id, const char *video_id);
const char *no_login_search_video(const char *search_key);
const char *update_privacy_respond(const char *video_id, const char *privacy);

int main()
{
    printf("%s\n", update_privacy_respond("5","private"));
    return 0;
}

const char *no_login_all_videos()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct json_object *no_login_all_videos = json_object_new_object();

    conn = mysql_init(NULL);

    char *query_string = "select * from youtok.videos where youtok.videos.privacy = \"public\";";

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, query_string))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_store_result(conn);

    struct json_object *videos_list = json_object_new_array();

    if (mysql_num_rows(res) > 0)
    {
        printf("successfully!\n");
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            char id[10];
            char title[255];
            char description[255];
            char privacy[30];
            char filename[255];

            strcpy(id, row[0]);
            strcpy(title, row[2]);
            strcpy(description, row[3]);
            strcpy(privacy, row[4]);
            strcpy(filename, row[5]);

            struct json_object *video = json_object_new_object();

            json_object_object_add(video, "id", json_object_new_string(id));
            json_object_object_add(video, "title", json_object_new_string(title));
            json_object_object_add(video, "description", json_object_new_string(description));
            json_object_object_add(video, "privacy", json_object_new_string(privacy));
            json_object_object_add(video, "filename", json_object_new_string(filename));

            json_object_array_add(videos_list, video);
        }
    }
    else
    {
        printf("failed!\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    struct json_object *body_object = json_object_new_object();

    json_object_object_add(body_object, "videos_list", videos_list);

    json_object_object_add(no_login_all_videos, "body", body_object);

    mysql_free_result(res);
    mysql_close(conn);

    return json_object_to_json_string(no_login_all_videos);
}

const char *login_all_videos(const char *user_id)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct json_object *login_all_videos = json_object_new_object();

    conn = mysql_init(NULL);

    char query_string[255];
    strcpy(query_string, "select * from youtok.videos where youtok.videos.privacy = \"public\" ");
    strcat(query_string, "union select * from youtok.videos where youtok.videos.user_id = ");
    strcat(query_string, user_id);
    strcat(query_string, ";");

    printf("%s\n", query_string);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, query_string))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_store_result(conn);

    struct json_object *videos_list = json_object_new_array();

    if (mysql_num_rows(res) > 0)
    {
        printf("successfully!\n");
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            char id[10];
            char title[255];
            char description[255];
            char privacy[30];
            char filename[255];

            strcpy(id, row[0]);
            strcpy(title, row[2]);
            strcpy(description, row[3]);
            strcpy(privacy, row[4]);
            strcpy(filename, row[5]);

            struct json_object *video = json_object_new_object();

            json_object_object_add(video, "id", json_object_new_string(id));
            json_object_object_add(video, "title", json_object_new_string(title));
            json_object_object_add(video, "description", json_object_new_string(description));
            json_object_object_add(video, "privacy", json_object_new_string(privacy));
            json_object_object_add(video, "filename", json_object_new_string(filename));

            json_object_array_add(videos_list, video);
        }
    }
    else
    {
        printf("failed!\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    struct json_object *body_object = json_object_new_object();

    json_object_object_add(body_object, "videos_list", videos_list);

    json_object_object_add(login_all_videos, "body", body_object);

    mysql_free_result(res);
    mysql_close(conn);

    return json_object_to_json_string(login_all_videos);
}

const char *current_user_all_videos(const char *user_id)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct json_object *current_user_all_videos = json_object_new_object();

    conn = mysql_init(NULL);

    char query_string[255];
    strcpy(query_string, "select * from youtok.videos where youtok.videos.user_id = ");
    strcat(query_string, user_id);
    strcat(query_string, ";");

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, query_string))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_store_result(conn);

    struct json_object *videos_list = json_object_new_array();

    if (mysql_num_rows(res) > 0)
    {
        printf("successfully!\n");
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            char id[10];
            char title[255];
            char description[255];
            char privacy[30];
            char filename[255];

            strcpy(id, row[0]);
            strcpy(title, row[2]);
            strcpy(description, row[3]);
            strcpy(privacy, row[4]);
            strcpy(filename, row[5]);

            struct json_object *video = json_object_new_object();

            json_object_object_add(video, "id", json_object_new_string(id));
            json_object_object_add(video, "title", json_object_new_string(title));
            json_object_object_add(video, "description", json_object_new_string(description));
            json_object_object_add(video, "privacy", json_object_new_string(privacy));
            json_object_object_add(video, "filename", json_object_new_string(filename));

            json_object_array_add(videos_list, video);
        }
    }
    else
    {
        printf("failed!\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    struct json_object *body_object = json_object_new_object();

    json_object_object_add(body_object, "videos_list", videos_list);

    json_object_object_add(current_user_all_videos, "body", body_object);

    mysql_free_result(res);
    mysql_close(conn);

    return json_object_to_json_string(current_user_all_videos);
}

int check_permission_to_watch_video(const char *user_id, const char *video_id)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct json_object *user_json = json_object_new_object();

    conn = mysql_init(NULL);

    char privacy[10];
    char query_string[255];
    int check = 1;

    strcpy(query_string, "select youtok.videos.privacy from youtok.videos where youtok.videos.id = ");
    strcat(query_string, video_id);
    strcat(query_string, " and youtok.videos.user_id <> ");
    strcat(query_string, user_id);
    strcat(query_string, ";");

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, query_string))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_store_result(conn);

    if (mysql_num_rows(res) > 0)
    {
        printf("successfully!\n");
        row = mysql_fetch_row(res);
        strcpy(privacy, row[0]);
        if (strcmp(privacy, "private") == 0)
            check = 0;
    }
    else
    {
        check = 0;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return check;
}

const char *update_privacy_respond(const char *video_id, const char *privacy)
{
    MYSQL *conn;
    MYSQL_RES *res;

    struct json_object *response_json = json_object_new_object();
    json_object_object_add(response_json, "head", json_object_new_string("update_privacy_response"));
    struct json_object *response_json_body = json_object_new_object();
    conn = mysql_init(NULL);

    char query_string[255];

    strcpy(query_string, "update youtok.videos set youtok.videos.privacy = \"");
    strcat(query_string, privacy);
    strcat(query_string, "\" where id = ");
    strcat(query_string, video_id);
    strcat(query_string, ";");

    printf("%s\n", query_string);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, query_string))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        json_object_object_add(response_json_body,"status",json_object_new_string("false"));
        json_object_object_add(response_json,"body",response_json_body);
        mysql_close(conn);
        return json_object_to_json_string(response_json);
    }

    json_object_object_add(response_json_body,"status",json_object_new_string("true"));
    json_object_object_add(response_json,"body",response_json_body);
    mysql_close(conn);
    return json_object_to_json_string(response_json);
}

const char *no_login_search_video(const char *search_key)
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    struct json_object *no_login_search_video = json_object_new_object();

    conn = mysql_init(NULL);

    char query_string[255];
    strcpy(query_string, "select * from youtok.videos where youtok.videos.title like \"%");
    strcat(query_string, search_key);
    strcat(query_string, "%\";");
    printf("%s\n", query_string);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    if (mysql_query(conn, query_string))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_store_result(conn);

    struct json_object *videos_list = json_object_new_array();

    if (mysql_num_rows(res) > 0)
    {
        printf("successfully!\n");
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            char id[10];
            char title[255];
            char description[255];
            char privacy[30];
            char filename[255];

            strcpy(id, row[0]);
            strcpy(title, row[2]);
            strcpy(description, row[3]);
            strcpy(privacy, row[4]);
            strcpy(filename, row[5]);

            struct json_object *video = json_object_new_object();

            json_object_object_add(video, "id", json_object_new_string(id));
            json_object_object_add(video, "title", json_object_new_string(title));
            json_object_object_add(video, "description", json_object_new_string(description));
            json_object_object_add(video, "privacy", json_object_new_string(privacy));
            json_object_object_add(video, "filename", json_object_new_string(filename));

            json_object_array_add(videos_list, video);
        }
    }
    else
    {
        printf("failed!\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    struct json_object *body_object = json_object_new_object();

    json_object_object_add(body_object, "videos_list", videos_list);

    json_object_object_add(no_login_search_video, "body", body_object);

    mysql_free_result(res);
    mysql_close(conn);

    return json_object_to_json_string(no_login_search_video);
}
