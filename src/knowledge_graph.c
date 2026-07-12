#define _CRT_SECURE_NO_WARNINGS  
#include "graph.h"               
#include "sqlite3.h"            
sqlite3* db;                     // ȫ��SQLite���ݿ�����ָ�룬���ж����ݿ�Ķ�д����Ҫͨ�����ָ�������
#define DB_FILENAME "ds_graph.db" // �������ݿ��ļ���
void printSQLError(char* desc) {
    // ��ӡ�������飺���� + SQLite������Ϣ + ������
    printf("�������ݿ� - %s | ���飺%s | �����룺%d\n",
        desc, sqlite3_errmsg(db), sqlite3_errcode(db));
}
//��ʼ��SQLite���ݿ⣨���������������ڣ�
int initSQLiteDB() {
    // ��һ���������ݿ⣨�������򴴽���
    int rc = sqlite3_open(DB_FILENAME, &db);
    if (rc != SQLITE_OK) { // ��ʧ��
        printSQLError("�����ݿ�ʧ��");
        return -1;
    }

    // �ڶ����������������graph_node��
    char* create_node_table = "create table if not exists graph_node (" // ����������������Ѿ����ڣ��Ͳ��ظ�����
        "id integer primary key,"          // ����ID��Ψһ��
        "name text not null,"              // �������ƣ��ǿգ�
        "type text not null);";            // �������ͣ��ǿգ�
    // ִ��SQL��䣨�޻ص�������
    rc = sqlite3_exec(db, create_node_table, NULL, NULL, NULL);// ִ��SQL��䣬���������ݿ����ӡ�Ҫִ�е�SQL���ص��������ص�������������Ϣ
    if (rc != SQLITE_OK) { // ������ʧ��
        printSQLError("���������ʧ��");
        return -1;
    }

    // �����߱�
    char* create_edge_table = "create table if not exists graph_edge ("
        "id integer primary key autoincrement," // �ߵ�����ID
        "start_id integer not null,"// ��㶥��
        "end_id integer not null,"// �յ㶥��
        "rel_name text not null,"// �ߵĹ�ϵ����
        "foreign key (start_id) references graph_node(id) on delete cascade,"// ��֤�ߵ����IDһ���Ƕ�����д��ڵ�ID��ɾ������ʱ�Զ�ɾ������
        "foreign key (end_id) references graph_node(id) on delete cascade);"; // ��֤�ߵ��յ�IDһ���Ƕ�����д��ڵ�ID��ɾ������ʱ�Զ�ɾ������

    // ִ�н������
    rc = sqlite3_exec(db, create_edge_table, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        printSQLError("�����߱�ʧ��");
        return -1;
    }

    printf("�ɹ������ݿ��ʼ����ɣ�\n");
    return 0;
}


//��ͼ���ݱ��浽SQLite���ݿ�
int saveGraphToSQLite(Graph* g) {
    // ��վ�����
    sqlite3_exec(db, "delete from graph_edge;", NULL, NULL, NULL); // ɾ���߱���������
    sqlite3_exec(db, "delete from graph_node;", NULL, NULL, NULL); // ɾ���������������

    // ���붥�����ݣ�����������ݣ���Ԥ�������
    sqlite3_stmt* stmt;// Ԥ����������
    
    char* sql_node = "insert into graph_node (id, name, type) values (?, ?, ?);";// SQLģ��
   
    // ����SQLģ�壬����Ԥ�������
    // ���������ݿ����ӡ�SQL��䡢SQL���ȣ�-1��ʾ�Զ����㣩��Ԥ�������ָ�롢δ������SQL��NULL��
    int rc = sqlite3_prepare_v2(db, sql_node, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError("Ԥ��������������ʧ��");
        return -1;
    }
    // �����ڴ�������ж��㣬����������ݿ�
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        // �󶨲�������SQLģ�����?��ֵ
        sqlite3_bind_int(stmt, 1, p->id);// ����1��?���ID���������ͣ�
        // SQLITE_TRANSIENT����ʾ��������ʱ�ģ�SQLite�踴��һ����ʱ�ã�
        sqlite3_bind_text(stmt, 2, p->name, strlen(p->name), SQLITE_TRANSIENT);// ��2��?������
        sqlite3_bind_text(stmt, 3, p->type, strlen(p->type), SQLITE_TRANSIENT);// ��3��?������
        sqlite3_step(stmt);//ִ��Ԥ�������
        sqlite3_reset(stmt); //����Ԥ�������
        p = p->next;
    }
    sqlite3_finalize(stmt);//�ͷ�Ԥ����������Դ

    // ���������
    char* sql_edge = "insert into graph_edge (start_id, end_id, rel_name) values (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql_edge, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError("Ԥ�����߲������ʧ��");
        return -1;
    }
    // �������ж���ıߣ�����������ݿ�
    p = g->vertex_head;
    while (p != NULL) {
        EdgeNode* e = p->first_edge;// ������ǰ��������б�
        while (e != NULL) {
            sqlite3_bind_int(stmt, 1, p->id);
            sqlite3_bind_int(stmt, 2, e->end_id);
            sqlite3_bind_text(stmt, 3, e->rel_name, strlen(e->rel_name), SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            e = e->next;// ������һ����
        }
        p = p->next;// ������һ������
    }
    sqlite3_finalize(stmt);

    printf("�ɹ���ͼ�������ѱ��浽���ݿ⣡\n");
    return 0;
}

//��SQLite���ݿ��������
int loadGraphFromSQLite(Graph* g) {
    //����ڴ����ͼ
    initGraph(g);
    // ��ѯ��������
    sqlite3_stmt* stmt; // Ԥ�������
    char* sql_node = "select id, name, type from graph_node;"; // ��ѯ���
    int rc = sqlite3_prepare_v2(db, sql_node, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError("Ԥ���������ѯ���ʧ��");
        return -1;
    }
    // ִ�в�ѯ���������
    while (sqlite3_step(stmt) == SQLITE_ROW) { //SQLITE_ROW����һ�����ݿ��Զ�ȡ
        // ��ȡ��ǰ�е�������
        int id = sqlite3_column_int(stmt, 0);
        char* name = (char*)sqlite3_column_text(stmt, 1);
        char* type = (char*)sqlite3_column_text(stmt, 2);
        addVertex(g, id, name, type);// �Ѷ�ȡ���Ķ������ݣ����ӵ��ڴ����ͼ�ṹ��
    }
    sqlite3_finalize(stmt);

    // ��ѯ������
    char* sql_edge = "select start_id, end_id, rel_name from graph_edge;";
    rc = sqlite3_prepare_v2(db, sql_edge, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError("Ԥ�����߲�ѯ���ʧ��");
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // ��ȡ������
        int start_id = sqlite3_column_int(stmt, 0);
        int end_id = sqlite3_column_int(stmt, 1);
        char* rel_name = (char*)sqlite3_column_text(stmt, 2);
        addEdge(g, start_id, end_id, rel_name);
    }
    sqlite3_finalize(stmt);

    printf("�ɹ������ݿ������Ѽ��ص�ͼ���У�\n");
    return 0;
}
//������
int main() {
    Graph my_graph;              // ����ͼ�ṹ�����
    initGraph(&my_graph);        // ��ʼ��ͼ���ÿգ�
    int choice;
    printf("======= ���ݽṹͼ�׹���ϵͳ =======\n");
    while (1) {
        // ��ӡ���ܲ˵�
        printf("\n==================== ���ܲ˵� ====================\n");
        printf("1. ���Ӷ���    2. ���ӱ�      3. ���Ҷ���/��\n");
        printf("4. �޸Ķ���    5. �޸ı�      6. ɾ������\n");
        printf("7. ɾ����      8. ����ͼ��    9. ���浽���ݿ�\n");
        printf("10. �����ݿ����   11. ͼ��ͳ��   0. �˳�����\n");
        printf("=================================================\n");
        printf("��ѡ���ܣ�0-12����");

        // ��ȡ�û�ѡ��������
        scanf("%d", &choice);
        getchar(); // ���ջ��з����������fgets��ȡ�����ַ���

        // ��֧�����������û�ѡ��ִ�ж�Ӧ����
        switch (choice) {
        case 1: { //���Ӷ���
            int id;
            char name[50], type[30];
            printf("���붥���ţ�"); scanf("%d", &id); getchar();
            printf("���붥�����ƣ�"); fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("���붥�����ͣ�"); fgets(type, sizeof(type), stdin);
            type[strcspn(type, "\n")] = '\0';
            addVertex(&my_graph, id, name, type); // ���Ӷ���
            break;
        }
        case 2: { //���ӱ�
            int start_id, end_id;
            char rel[50];
            printf("������㶥���ţ�"); scanf("%d", &start_id); getchar();
            printf("�����յ㶥���ţ�"); scanf("%d", &end_id); getchar();
            printf("�����ϵ���ƣ�"); fgets(rel, sizeof(rel), stdin);
            rel[strcspn(rel, "\n")] = '\0';
            addEdge(&my_graph, start_id, end_id, rel); //���ӱ�
            break;
        }
        case 3: { //���Ҷ���/��
            int sub_choice; //�Ӳ˵�ѡ��
            printf("\n---------------- �����Ӳ˵� ----------------\n");
            printf("1. ����Ų鶥��  2. �����Ʋ鶥��  3. �����Ͳ鶥��\n");
            printf("4. ����Ų鶥�������  5. �����Ʋ鶥�������\n");
            printf("-------------------------------------------\n");
            printf("ѡ���ӹ��ܣ�"); scanf("%d", &sub_choice); getchar();
            if (sub_choice == 1) { // ����Ų鶥��
                int id;
                printf("���붥���ţ�"); scanf("%d", &id); getchar();
                VertexNode* v = findVertexById(&my_graph, id);
                if (v != NULL) { // �ҵ�����
                    printf("�ҵ����㣺���=%d | ����=%s | ����=%s\n", v->id, v->name, v->type);
                }
                else { // δ�ҵ�
                    printf("ʧ�ܣ�δ�ҵ����=%d�Ķ���\n", id);
                }
            }
            else if (sub_choice == 2) { // �����Ʋ鶥��
                char name[50];
                printf("���붥�����ƹؼ��ʣ�"); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                findVertexByName(&my_graph, name); //�����Ʋ���
            }
            else if (sub_choice == 3) { //�����Ͳ鶥��
                char type[30];
                printf("���붥�����ͣ�"); fgets(type, sizeof(type), stdin);
                type[strcspn(type, "\n")] = '\0';
                findVertexByType(&my_graph, type); //�����Ͳ���
            }
            else if (sub_choice == 4) { //�鶥�������
                int id;
                printf("���붥���ţ�"); scanf("%d", &id); getchar();
                findVertexEdges(&my_graph, id); //���ҹ�����
            }
            else if (sub_choice == 5) { // �����������Ʋ������
                char name[50];
                printf("���붥�����ƹؼ��ʣ�"); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                findVertexEdgesByName(&my_graph, name);
            }
            else { //��Ч��ѡ��
                printf("ʧ�ܣ���ѡ��1-4���ӹ��ܣ�\n");
            }
            break;
        }
        case 4: { // 4. �޸Ķ���
            int id;
            char new_name[50], new_type[30];
            printf("����Ҫ�޸ĵĶ����ţ�"); scanf("%d", &id); getchar();
            printf("���������ƣ�"); fgets(new_name, sizeof(new_name), stdin);
            new_name[strcspn(new_name, "\n")] = '\0';
            printf("���������ͣ�"); fgets(new_type, sizeof(new_type), stdin);
            new_type[strcspn(new_type, "\n")] = '\0';
            modifyVertex(&my_graph, id, new_name, new_type);
            break;
        }
        case 5: { //�޸ı�
            int start_id, end_id;
            char new_rel[50];
            printf("����Ҫ�޸ĵıߣ�����ţ���"); scanf("%d", &start_id); getchar();
            printf("����Ҫ�޸ĵıߣ��յ��ţ���"); scanf("%d", &end_id); getchar();
            printf("�����¹�ϵ���ƣ�"); fgets(new_rel, sizeof(new_rel), stdin);
            new_rel[strcspn(new_rel, "\n")] = '\0';
            modifyEdge(&my_graph, start_id, end_id, new_rel);
            break;
        }
        case 6: { //ɾ������
            int id;
            printf("����Ҫɾ���Ķ����ţ�"); scanf("%d", &id); getchar();
            deleteVertex(&my_graph, id); 
            break;
        }
        case 7: { // 7. ɾ����
            int start_id, end_id;
            printf("����Ҫɾ���ıߣ�����ţ���"); scanf("%d", &start_id); getchar();
            printf("����Ҫɾ���ıߣ��յ��ţ���"); scanf("%d", &end_id); getchar();
            deleteEdge(&my_graph, start_id, end_id);
            break;
        }
        case 8: { //����ͼ��
            traverseGraph(&my_graph);
            break;
        }
        case 9: { //���浽���ݿ�
            if (initSQLiteDB() == 0) { //�ȳ�ʼ�����ݿ�
                saveGraphToSQLite(&my_graph); //��������
                sqlite3_close(db); //�ر����ݿ�����
            }
            break;
        }
        case 10: { //�����ݿ����
            if (initSQLiteDB() == 0) { //��ʼ�����ݿ�
                loadGraphFromSQLite(&my_graph); //��������
                sqlite3_close(db); //�ر����ݿ�����
            }
            break;
        }
        case 11: { //ͼ��ͳ��
            printGraphStats(&my_graph);
            break;
        }
        case 12: { //����
            system("cls");

            break;
        }
        case 0: { // 0. �˳�����
            printf("�����˳��У������ͷ��ڴ�...\n");
            // �ͷ����ж���ͱߵ��ڴ�
            VertexNode* p = my_graph.vertex_head;
            while (p != NULL) {
                VertexNode* temp_v = p; //��ʱ���浱ǰ����
                EdgeNode* e = p->first_edge; //�ͷŵ�ǰ��������б�
                while (e != NULL) {
                    EdgeNode* temp_e = e;
                    e = e->next;
                    free(temp_e); //�ͷű��ڴ�
                }
                p = p->next; //������һ������
                free(temp_v); //�ͷŶ����ڴ�
            }
            printf("�������˳���\n");
            return 0;
        }
        default:
            printf("ʧ�ܣ���ѡ��0-12�Ĺ��ܣ�\n");
        }
    }
}