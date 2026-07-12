#define _CRT_SECURE_NO_WARNINGS
#include "graph.h"
#include "sqlite3.h"

#define DB_FILENAME "ds_graph.db"  // 数据库文件名

/* ==================== 菜单枚举 ==================== */
typedef enum {
    MENU_EXIT = 0,
    MENU_ADD_VERTEX = 1,
    MENU_ADD_EDGE,
    MENU_QUERY_VERTEX,
    MENU_MODIFY_VERTEX,
    MENU_MODIFY_EDGE,
    MENU_DELETE_VERTEX,
    MENU_DELETE_EDGE,
    MENU_TRAVERSE,
    MENU_SAVE_DB,
    MENU_LOAD_DB,
    MENU_STATS,
    MENU_CLEAR
} MenuChoice;

typedef enum {
    QUERY_BY_ID = 1,
    QUERY_BY_NAME,
    QUERY_BY_TYPE,
    QUERY_EDGES,
    QUERY_EDGES_BY_NAME
} QueryChoice;

/* ==================== SQLite 辅助函数 ==================== */
void printSQLError(sqlite3* db, const char* desc) {
    printf("操作数据库 - %s | 信息：%s | 错误码：%d\n",
        desc, sqlite3_errmsg(db), sqlite3_errcode(db));
}

int execSQLWithError(sqlite3* db, const char* sql, const char* desc) {
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        printf("操作数据库 - %s | 详情：%s\n", desc, err_msg);
        sqlite3_free(err_msg);
    }
    return rc;
}

/* ==================== SQLite 初始化 ==================== */
int initSQLiteDB(sqlite3** p_db) {
    int rc = sqlite3_open(DB_FILENAME, p_db);
    if (rc != SQLITE_OK) {
        printf("操作数据库 - 打开数据库失败 | 信息：%s | 错误码：%d\n",
            sqlite3_errmsg(*p_db), sqlite3_errcode(*p_db));
        return -1;
    }

    const char* create_node_table =
        "create table if not exists graph_node ("
        "id integer primary key,"
        "name text not null,"
        "type text not null);";

    rc = execSQLWithError(*p_db, create_node_table, "创建节点表失败");
    if (rc != SQLITE_OK) return -1;

    const char* create_edge_table =
        "create table if not exists graph_edge ("
        "id integer primary key autoincrement,"
        "start_id integer not null,"
        "end_id integer not null,"
        "rel_name text not null,"
        "foreign key (start_id) references graph_node(id) on delete cascade,"
        "foreign key (end_id) references graph_node(id) on delete cascade);";

    rc = execSQLWithError(*p_db, create_edge_table, "创建边表失败");
    if (rc != SQLITE_OK) return -1;

    printf("成功：数据库初始化完成！\n");
    return 0;
}

/* ==================== 保存图到 SQLite ==================== */
int saveGraphToSQLite(sqlite3* db, Graph* g) {
    execSQLWithError(db, "delete from graph_edge;", "清空边表失败");
    execSQLWithError(db, "delete from graph_node;", "清空节点表失败");

    /* 插入顶点数据 */
    sqlite3_stmt* stmt;
    const char* sql_node = "insert into graph_node (id, name, type) values (?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sql_node, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError(db, "预编译顶点插入失败");
        return -1;
    }

    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        sqlite3_bind_int(stmt, 1, p->id);
        sqlite3_bind_text(stmt, 2, p->name, strlen(p->name), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, p->type, strlen(p->type), SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        p = p->next;
    }
    sqlite3_finalize(stmt);

    /* 插入边数据 */
    const char* sql_edge = "insert into graph_edge (start_id, end_id, rel_name) values (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql_edge, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError(db, "预编译边插入失败");
        return -1;
    }

    p = g->vertex_head;
    while (p != NULL) {
        EdgeNode* e = p->first_edge;
        while (e != NULL) {
            sqlite3_bind_int(stmt, 1, p->id);
            sqlite3_bind_int(stmt, 2, e->end_id);
            sqlite3_bind_text(stmt, 3, e->rel_name, strlen(e->rel_name), SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            e = e->next;
        }
        p = p->next;
    }
    sqlite3_finalize(stmt);

    printf("成功：图数据已保存到数据库！\n");
    return 0;
}

/* ==================== 从 SQLite 加载图 ==================== */
int loadGraphFromSQLite(sqlite3* db, Graph* g) {
    initGraph(g);

    /* 加载顶点 */
    sqlite3_stmt* stmt;
    const char* sql_node = "select id, name, type from graph_node;";
    int rc = sqlite3_prepare_v2(db, sql_node, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError(db, "预编译顶点查询失败");
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = (const char*)sqlite3_column_text(stmt, 1);
        const char* type = (const char*)sqlite3_column_text(stmt, 2);
        addVertex(g, id, (char*)name, (char*)type);
    }
    sqlite3_finalize(stmt);

    /* 加载边 */
    const char* sql_edge = "select start_id, end_id, rel_name from graph_edge;";
    rc = sqlite3_prepare_v2(db, sql_edge, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printSQLError(db, "预编译边查询失败");
        return -1;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int start_id = sqlite3_column_int(stmt, 0);
        int end_id = sqlite3_column_int(stmt, 1);
        const char* rel_name = (const char*)sqlite3_column_text(stmt, 2);
        addEdge(g, start_id, end_id, (char*)rel_name);
    }
    sqlite3_finalize(stmt);

    printf("成功：已从数据库加载图数据！\n");
    return 0;
}

/* ==================== 主菜单 ==================== */
void showMenu() {
    printf("\n======================================\n");
    printf("          知识图谱系统菜单\n");
    printf("======================================\n");
    printf("  %2d. 添加顶点\n", MENU_ADD_VERTEX);
    printf("  %2d. 添加边\n", MENU_ADD_EDGE);
    printf("  %2d. 查询顶点\n", MENU_QUERY_VERTEX);
    printf("  %2d. 修改顶点\n", MENU_MODIFY_VERTEX);
    printf("  %2d. 修改边\n", MENU_MODIFY_EDGE);
    printf("  %2d. 删除顶点\n", MENU_DELETE_VERTEX);
    printf("  %2d. 删除边\n", MENU_DELETE_EDGE);
    printf("  %2d. 浏览图\n", MENU_TRAVERSE);
    printf("  %2d. 保存到数据库\n", MENU_SAVE_DB);
    printf("  %2d. 从数据库加载\n", MENU_LOAD_DB);
    printf("  %2d. 图统计\n", MENU_STATS);
    printf("  %2d. 清屏\n", MENU_CLEAR);
    printf("  %2d. 退出\n", MENU_EXIT);
    printf("--------------------------------------\n");
    printf("  请输入功能编号：");
}

void showQuerySubMenu() {
    printf("\n  --- 查询子菜单 ---\n");
    printf("    %d. 按编号查询\n", QUERY_BY_ID);
    printf("    %d. 按名称查询\n", QUERY_BY_NAME);
    printf("    %d. 按类型查询\n", QUERY_BY_TYPE);
    printf("    %d. 查询顶点关联边\n", QUERY_EDGES);
    printf("    %d. 按名称查询关联边\n", QUERY_EDGES_BY_NAME);
    printf("  请输入查询方式：");
}

void freeGraphMemory(Graph* g) {
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        VertexNode* temp_v = p;
        EdgeNode* e = p->first_edge;
        while (e != NULL) {
            EdgeNode* temp_e = e;
            e = e->next;
            free(temp_e);
        }
        p = p->next;
        free(temp_v);
    }
}

/* ==================== 程序入口 ==================== */
int main() {
    Graph my_graph;
    initGraph(&my_graph);
    int choice;

    while (1) {
        showMenu();
        scanf("%d", &choice);
        getchar();

        switch ((MenuChoice)choice) {
        case MENU_ADD_VERTEX: {
            int id;
            char name[50], type[30];
            printf("请输入顶点编号："); scanf("%d", &id); getchar();
            printf("请输入顶点名称："); fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            printf("请输入顶点类型："); fgets(type, sizeof(type), stdin);
            type[strcspn(type, "\n")] = '\0';
            addVertex(&my_graph, id, name, type);
            break;
        }
        case MENU_ADD_EDGE: {
            int start_id, end_id;
            char rel_name[50];
            printf("请输入起点编号："); scanf("%d", &start_id); getchar();
            printf("请输入终点编号："); scanf("%d", &end_id); getchar();
            printf("请输入关系名称："); fgets(rel_name, sizeof(rel_name), stdin);
            rel_name[strcspn(rel_name, "\n")] = '\0';
            addEdge(&my_graph, start_id, end_id, rel_name);
            break;
        }
        case MENU_QUERY_VERTEX: {
            int sub_choice;
            showQuerySubMenu();
            scanf("%d", &sub_choice);
            getchar();

            switch ((QueryChoice)sub_choice) {
            case QUERY_BY_ID: {
                int id;
                printf("请输入顶点编号："); scanf("%d", &id); getchar();
                VertexNode* v = findVertexById(&my_graph, id);
                if (v != NULL)
                    printf("  编号：%d | 名称：%s | 类型：%s\n", v->id, v->name, v->type);
                else
                    printf("失败：未找到编号为%d的顶点\n", id);
                break;
            }
            case QUERY_BY_NAME: {
                char name[50];
                printf("请输入顶点名称："); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                findVertexByName(&my_graph, name);
                break;
            }
            case QUERY_BY_TYPE: {
                char type[30];
                printf("请输入顶点类型："); fgets(type, sizeof(type), stdin);
                type[strcspn(type, "\n")] = '\0';
                findVertexByType(&my_graph, type);
                break;
            }
            case QUERY_EDGES: {
                int id;
                printf("请输入顶点编号："); scanf("%d", &id); getchar();
                findVertexEdges(&my_graph, id);
                break;
            }
            case QUERY_EDGES_BY_NAME: {
                char name[50];
                printf("请输入顶点名称关键字："); fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                findVertexEdgesByName(&my_graph, name);
                break;
            }
            default:
                printf("失败：请选择1-5的查询功能\n");
                break;
            }
            break;
        }
        case MENU_MODIFY_VERTEX: {
            int id;
            char new_name[50], new_type[30];
            printf("请输入要修改的顶点编号："); scanf("%d", &id); getchar();
            printf("请输入新名称："); fgets(new_name, sizeof(new_name), stdin);
            new_name[strcspn(new_name, "\n")] = '\0';
            printf("请输入新类型："); fgets(new_type, sizeof(new_type), stdin);
            new_type[strcspn(new_type, "\n")] = '\0';
            modifyVertex(&my_graph, id, new_name, new_type);
            break;
        }
        case MENU_MODIFY_EDGE: {
            int start_id, end_id;
            char new_rel[50];
            printf("请输入要修改的边（起点编号）："); scanf("%d", &start_id); getchar();
            printf("请输入要修改的边（终点编号）："); scanf("%d", &end_id); getchar();
            printf("请输入新关系名称："); fgets(new_rel, sizeof(new_rel), stdin);
            new_rel[strcspn(new_rel, "\n")] = '\0';
            modifyEdge(&my_graph, start_id, end_id, new_rel);
            break;
        }
        case MENU_DELETE_VERTEX: {
            int id;
            printf("请输入要删除的顶点编号："); scanf("%d", &id); getchar();
            deleteVertex(&my_graph, id);
            break;
        }
        case MENU_DELETE_EDGE: {
            int start_id, end_id;
            printf("请输入要删除的边（起点编号）："); scanf("%d", &start_id); getchar();
            printf("请输入要删除的边（终点编号）："); scanf("%d", &end_id); getchar();
            deleteEdge(&my_graph, start_id, end_id);
            break;
        }
        case MENU_TRAVERSE: {
            traverseGraph(&my_graph);
            break;
        }
        case MENU_SAVE_DB: {
            sqlite3* db = NULL;
            if (initSQLiteDB(&db) == 0) {
                saveGraphToSQLite(db, &my_graph);
                sqlite3_close(db);
            }
            break;
        }
        case MENU_LOAD_DB: {
            sqlite3* db = NULL;
            if (initSQLiteDB(&db) == 0) {
                loadGraphFromSQLite(db, &my_graph);
                sqlite3_close(db);
            }
            break;
        }
        case MENU_STATS: {
            printGraphStats(&my_graph);
            break;
        }
        case MENU_CLEAR: {
            system("cls");
            break;
        }
        case MENU_EXIT: {
            printf("正在退出，释放内存...\n");
            freeGraphMemory(&my_graph);
            printf("程序已退出！\n");
            return 0;
        }
        default:
            printf("失败：请选择0-12的功能\n");
            break;
        }
    }
}
