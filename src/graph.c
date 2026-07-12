#define _CRT_SECURE_NO_WARNINGS  
#include "graph.h"
// ��ʼ��ͼ�ṹ
void initGraph(Graph* g) {
    g->vertex_head = NULL;       // ��������ͷָ���ÿ�
    g->vertex_count = 0;         // ����������ʼ��Ϊ0
    g->edge_count = 0;           // ��������ʼ��Ϊ0
}
//����Ų��Ҷ���
VertexNode* findVertexById(Graph* g, int id) {
    VertexNode* p = g->vertex_head; // �Ӷ�������ͷ�ڵ㿪ʼ����
    while (p != NULL) {            
        if (p->id == id) {          
            return p;               // �����ҵ��Ķ���ָ��
        }
        p = p->next;               
    }
    return NULL;                 
}
//���Ӷ���
int addVertex(Graph* g, int id, char* name, char* type) {
    //���ID�Ƿ��ظ�
    if (findVertexById(g, id) != NULL) {
        printf("ʧ�ܣ�������=%d�Ѵ��ڣ�����ʧ�ܣ�\n", id);
        return -1;
    }
    //�����ڴ洴���¶���ڵ�
    VertexNode* new_vertex = (VertexNode*)malloc(sizeof(VertexNode));
    new_vertex->id = id;                       // ��ֵ����ID
    strcpy(new_vertex->name, name);            // ��ֵ��������
    strcpy(new_vertex->type, type);            // ��ֵ��������
    new_vertex->first_edge = NULL;             // ������ͷָ���ÿ�
    new_vertex->next = NULL;                   // ��next�ÿգ���Ϊ����β

    //β�巨���붥������
    if (g->vertex_head == NULL) {              // ��������Ϊ��
        g->vertex_head = new_vertex;           // �¶���ֱ����Ϊͷ�ڵ�
    }
    else {                                  
        VertexNode* last = g->vertex_head;     // �ӱ�ͷ��ʼ��β�ڵ�
        while (last->next != NULL) {      
            last = last->next;
        }
        last->next = new_vertex;               // �¶������β�ڵ��
    }
    g->vertex_count++;                         // ������+1
    printf("�ɹ����������ӳɹ���\n");
    printf("  ��ţ�%d | ���ƣ�%s | ���ͣ�%s\n", id, name, type);
    return 0; // ���ӳɹ�������0
}
//���ӱ�
int addEdge(Graph* g, int start_id, int end_id, char* rel_name) {
    VertexNode* start_vertex = findVertexById(g, start_id); // ������㶥��
    VertexNode* end_vertex = findVertexById(g, end_id);     // �����յ㶥��
    if (start_vertex == NULL || end_vertex == NULL) {
        printf("ʧ�ܣ������յ㶥�㲻���ڣ�������ʧ�ܣ�\n");
        return -1; // ����ʧ����
    }
    // �ڶ����������Ƿ��ظ�
    EdgeNode* e = start_vertex->first_edge;
    while (e != NULL) {                  
        if (e->end_id == end_id) {      
            printf("ʧ�ܣ���%d->%d�Ѵ��ڣ������ظ����ӣ�\n", start_id, end_id);
            return -1;
        }
        e = e->next;
    }
    //�����ڴ洴���±߽ڵ�
    EdgeNode* new_edge = (EdgeNode*)malloc(sizeof(EdgeNode));
    new_edge->end_id = end_id;               
    strcpy(new_edge->rel_name, rel_name);   
    new_edge->next = NULL;          

    //β�巨������㶥��ı�����
    if (start_vertex->first_edge == NULL) { 
        start_vertex->first_edge = new_edge;
    }
    else {                               
        EdgeNode* last = start_vertex->first_edge;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_edge; 
    }
    g->edge_count++; // ����+1
    printf("�ɹ��������ӳɹ���\n");
    printf("  %d��%s��-> %s -> %d��%s��\n",
        start_id, start_vertex->name, rel_name, end_id, end_vertex->name);
    return 0;
}
//�����Ʋ��Ҷ���
void findVertexByName(Graph* g, char* name) {
    int count = 0; // ��¼ƥ�䵽�Ķ�������
    printf("\n------------------------------\n");
    printf("�������ư�����%s���Ķ���\n", name);
    printf("------------------------------\n");
    // �������ж���
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        if (strstr(p->name, name) != NULL) { //�ж�name�Ƿ���p->name���Ӵ�
            // ��ӡƥ�䵽�Ķ�����Ϣ
            printf("  ��ţ�%d | ���ƣ�%s | ���ͣ�%s\n", p->id, p->name, p->type);
            count++; // ƥ����+1
        }
        p = p->next; // ������һ������
    }
    if (count == 0) {
        printf("  ��ʾ��δ�ҵ�ƥ�䶥��\n");
    }
    else {
        printf("  ��ʾ�����ҵ�%d��ƥ�䶥��\n", count);
    }
    printf("------------------------------\n");
}
//�����;�ȷ���Ҷ���
void findVertexByType(Graph* g, char* type) {
    int count = 0; // ��¼ƥ�䵽�Ķ�������
    printf("\n------------------------------\n");
    printf("��������Ϊ��%s���Ķ���\n", type);
    printf("------------------------------\n");
    // �������ж���
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        if (strcmp(p->type, type) == 0) {
            printf("  ��ţ�%d | ���ƣ�%s | ���ͣ�%s\n", p->id, p->name, p->type);
            count++; // ƥ����+1
        }
        p = p->next;
    }
    if (count == 0) {
        printf("  ��ʾ��δ�ҵ������͵Ķ���\n");
    }
    else {
        printf("  ��ʾ�����ҵ�%d����%s�����Ͷ���\n", count, type);
    }
    printf("------------------------------\n");
}
//���Ҷ�������й�����
void findVertexEdges(Graph* g, int vertex_id) {
    //����Ŀ�궥��
    VertexNode* target = findVertexById(g, vertex_id);
    if (target == NULL) {
        printf("ʧ�ܣ�δ�ҵ����=%d�Ķ���\n", vertex_id);
        return;
    }
    int out_count = 0, in_count = 0; // ����/��߼���
    printf("\n------------------------------\n");
    printf("������=%d��%s���Ĺ���������\n", vertex_id, target->name);
    printf("------------------------------\n");
    printf("  ���ߣ��Ӹö����������\n");
    EdgeNode* e = target->first_edge; //����
    while (e != NULL) {
        // ���ұߵ��յ㶥��
        VertexNode* end_vertex = findVertexById(g, e->end_id);
        char end_name[50] = "δ֪����"; // Ĭ������
        if (end_vertex != NULL) {
            strcpy(end_name, end_vertex->name);
        }
        printf("    - %d��%s��-> %s -> %s�����=%d��\n", vertex_id, target->name, e->rel_name, end_name, e->end_id);
        out_count++;
        e = e->next;
    }
    if (out_count == 0) {
        printf("    - �޳���\n");
    }
    printf("\n  ��ߣ�ָ��ö��㣩��\n");
    VertexNode* p = g->vertex_head; // �������ж���
    while (p != NULL) {
        EdgeNode* e_in = p->first_edge; // ������ǰ��������г���
        while (e_in != NULL) {
            if (e_in->end_id == vertex_id) { // �ҵ�ָ��Ŀ�궥��ı�
                printf("    - %s�����=%d��-> %s -> %d��%s��\n",
                    p->name, p->id, e_in->rel_name, vertex_id, target->name);
                in_count++; // ��߼���+1
            }
            e_in = e_in->next;
        }
        p = p->next;
    }
    if (in_count == 0) { 
        printf("    - �����\n");
    }
    printf("\n  ������ͳ�ƣ�����%d�� | ���%d�� | �ܼ�%d��\n", out_count, in_count, out_count + in_count);
    printf("------------------------------\n");
}
//�޸Ķ�������ƺ�����
int modifyVertex(Graph* g, int id, char* new_name, char* new_type) {
    // ��һ��������Ŀ�궥��
    VertexNode* vertex = findVertexById(g, id);
    if (vertex == NULL) { // ���㲻����
        printf("ʧ�ܣ�δ�ҵ����=%d�Ķ��㣬�޸�ʧ�ܣ�\n", id);
        return -1; // ����ʧ����
    }

    // �ڶ���������ԭ��Ϣ��������ʾ�û��޸�ǰ��ĶԱȣ�
    char old_name[50], old_type[30];
    strcpy(old_name, vertex->name); // ����ԭ����
    strcpy(old_type, vertex->type); // ����ԭ����

    // �����������¶�����Ϣ
    strcpy(vertex->name, new_name); // ��ֵ������
    strcpy(vertex->type, new_type); // ��ֵ������

    // ���Ĳ�����ӡ�޸ĳɹ���ʾ
    printf("�ɹ��������޸ĳɹ���\n");
    printf("  ��ţ�%d | ԭ���ƣ�%s �� �����ƣ�%s\n", id, old_name, new_name);
    printf("  ��ţ�%d | ԭ���ͣ�%s �� �����ͣ�%s\n", id, old_type, new_type);
    return 0; // �޸ĳɹ�������0
}
void findVertexEdgesByName(Graph* g, char* name) {
    int count = 0; // ��¼ƥ�䵽�Ķ�������
    // ��ӡ����
    printf("\n------------------------------\n");
    printf("�������ư�����%s���Ķ���Ĺ�����\n", name);
    printf("------------------------------\n");

    // �������ж��㣬ģ��ƥ������
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        if (strstr(p->name, name) != NULL) { // ���ư����ؼ��ʼ�ƥ��
            count++;
            // ��ӡ��ǰƥ�䶥�����Ϣ
            printf("\n��ƥ�䵽��%d�����㡿\n", count);
            printf("  �����ţ�%d | ���ƣ�%s | ���ͣ�%s\n", p->id, p->name, p->type);
            // ����ԭ�еİ�ID��ߺ�������ӡ�ö���Ĺ�����
            findVertexEdges(g, p->id);
        }
        p = p->next;
    }
    if (count == 0) {
        printf("  ��ʾ��δ�ҵ����ư�����%s���Ķ���\n", name);
    }
    else {
        printf("\n------------------------------\n");
        printf("  ��ʾ�����ҵ�%d������ƥ��Ķ��㣬�Ѵ�ӡ���й�����\n", count);
        printf("------------------------------\n");
    }
}
 //�޸ıߵĹ�ϵ����
int modifyEdge(Graph* g, int start_id, int end_id, char* new_rel_name) {
    //������㶥��
    VertexNode* start_vertex = findVertexById(g, start_id);
    if (start_vertex == NULL) {
        printf("ʧ�ܣ������=%d�Ķ��㲻���ڣ��޸�ʧ�ܣ�\n", start_id);
        return -1;
    }
    //�������ı�����������Ŀ���
    EdgeNode* e = start_vertex->first_edge;
    while (e != NULL) {
        if (e->end_id == end_id) {
            // ����ԭ��ϵ����
            char old_rel[50];
            strcpy(old_rel, e->rel_name);
            // ���¹�ϵ����
            strcpy(e->rel_name, new_rel_name);
            printf("�ɹ������޸ĳɹ���\n");
            printf("  %d->%d | ԭ��ϵ��%s �� �¹�ϵ��%s\n", start_id, end_id, old_rel, new_rel_name);
            return 0;
        }
        e = e->next;
    }
    printf("ʧ�ܣ�δ�ҵ���%d->%d���޸�ʧ�ܣ�\n", start_id, end_id);
    return -1;
}
//ɾ������
int deleteVertex(Graph* g, int id) {
    //����Ŀ�궥���ǰ������
    VertexNode* target = NULL;  // Ŀ�궥��ָ��
    VertexNode* prev = NULL;    // Ŀ�궥���ǰ������ָ��
    VertexNode* p = g->vertex_head; 
    while (p != NULL) {
        if (p->id == id) {     
            target = p;
            break;
        }
        prev = p;               // ����ǰ������
        p = p->next;            // ������һ������
    }
    if (target == NULL) {
        printf("ʧ�ܣ�������=%d�����ڣ�ɾ��ʧ�ܣ�\n", id);
        return -1;
    }
    // ɾ��Ŀ�궥������г���
    EdgeNode* e = target->first_edge; // �ӵ�һ�����߿�ʼ
    while (e != NULL) {
        EdgeNode* temp = e;    // ��ʱ���浱ǰ�߽ڵ�
        e = e->next;    
        free(temp);            // �ͷŵ�ǰ�߽ڵ��ڴ�
        g->edge_count--;       // ����-1
    }
    //ɾ������ָ��ö�������
    p = g->vertex_head;        // �������ж���
    while (p != NULL) {
        if (p->id == id) {     // ��������
            p = p->next;
            continue;
        }

        EdgeNode* e_in = p->first_edge; // ������ǰ����ı�����
        EdgeNode* prev_e = NULL;        // ��ߵ�ǰ���ڵ�ָ��
        while (e_in != NULL) {
            if (e_in->end_id == id) {   // �ҵ�ָ��Ŀ�궥������
                // �Ƴ�����߽ڵ�
                if (prev_e == NULL) {  // ����Ǳ�����ͷ�ڵ�
                    p->first_edge = e_in->next;
                }
                else {               // ������м�/β�ڵ�
                    prev_e->next = e_in->next;
                }
                free(e_in);            // �ͷ�����ڴ�
                g->edge_count--;       // ����-1
                break;       
            }
            prev_e = e_in;             // ����ǰ����
            e_in = e_in->next;       
        }
        p = p->next;
    }
    //�Ӷ���������ɾ��Ŀ�궥��
    if (prev == NULL) {        // Ŀ�궥����ͷ�ڵ�
        g->vertex_head = target->next; // ͷָ��ָ��Ŀ�����һ������
    }
    else {                   // Ŀ�궥�����м�/β�ڵ�
        prev->next = target->next;     // ǰ������ָ��Ŀ�����һ������
    }
    free(target);              // �ͷŶ����ڴ�
    g->vertex_count--;         // ������-1
    printf("�ɹ���������=%d�����й�����ɾ���ɹ���\n", id);
    return 0;
}
 //ɾ��ָ���ıߣ������յ㣩
int deleteEdge(Graph* g, int start_id, int end_id) {
    // ��һ����������㶥��
    VertexNode* start_vertex = findVertexById(g, start_id);
    if (start_vertex == NULL) {
        printf("ʧ�ܣ������=%d�Ķ��㲻���ڣ�ɾ��ʧ�ܣ�\n", start_id);
        return -1;
    }
    //�������ı�����������Ŀ���
    EdgeNode* e = start_vertex->first_edge; 
    EdgeNode* prev_e = NULL;                // �ߵ�ǰ���ڵ�ָ��
    while (e != NULL) {
        if (e->end_id == end_id) {         
            //�Ƴ��ñ߽ڵ�
            if (prev_e == NULL) {           // ����ͷ�ڵ�
                start_vertex->first_edge = e->next;
            }
            else {                        // �����м�/β�ڵ�
                prev_e->next = e->next;
            }
            free(e);                       
            g->edge_count--;                // ����-1
            printf("�ɹ�����ɾ���ɹ� - %d->%d\n", start_id, end_id);
            return 0;
        }
        prev_e = e;
        e = e->next;
    }
    printf("ʧ�ܣ�δ�ҵ���%d->%d��ɾ��ʧ�ܣ�\n", start_id, end_id);
    return -1;
}
 //��������ӡ����ͼ�Ľṹ
void traverseGraph(Graph* g) {
    if (g->vertex_head == NULL) {
        printf("\n------------------------------\n");
        printf("ͼ�ṹ���飨��ͼ��\n");
        printf("------------------------------\n");
        printf("��ʾ����ǰͼ�����κζ���ͱ�\n");
        printf("------------------------------\n");
        return;
    }
    printf("\n------------------------------\n");
    printf("ͼ�ṹ���飨��������%d��������%d��\n", g->vertex_count, g->edge_count);
    printf("------------------------------\n");
    printf("  ���ж��㣺\n");
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        printf("    ��ţ�%d | ���ƣ�%s | ���ͣ�%s\n", p->id, p->name, p->type);
        p = p->next;
    }
    printf("\n  ���бߣ�\n");
    int edge_idx = 1; // �ߵ����
    p = g->vertex_head; // �������ж���
    while (p != NULL) {
        EdgeNode* e = p->first_edge; // ������ǰ��������б�
        while (e != NULL) {
            // ���ұߵ��յ㶥�㣨��ֹ�յ㲻���ڣ�
            VertexNode* end_vertex = findVertexById(g, e->end_id);
            char end_name[50] = "δ֪����"; // Ĭ������
            if (end_vertex != NULL) {
                strcpy(end_name, end_vertex->name);
            }
            printf("    %d. %s����ţ�%d��-> %s -> %s����ţ�%d��\n",
                edge_idx++, p->name, p->id, e->rel_name, end_name, e->end_id);
            e = e->next; 
        }
        p = p->next;
    }
    printf("------------------------------\n");
}
 //��ӡͼ��ͳ����Ϣ�����������������������ͷֲ���
void printGraphStats(Graph* g) {
    printf("\n------------------------------\n");
    printf("ͼ��ͳ����Ϣ\n");
    printf("------------------------------\n");
    printf("  1. ����������%d\n", g->vertex_count);
    printf("  2. ��������%d\n", g->edge_count);
    printf("  3. �������ͷֲ���\n");
    char types[100][30]; // �洢���в�ͬ�Ķ�������
    int counts[100] = { 0 }; // ��Ӧ���͵Ķ�������
    int type_num = 0; // ���͵�����
    // ͳ������
    VertexNode* p = g->vertex_head;
    while (p != NULL) {
        int found = 0; // ��Ǹ�����
        for (int i = 0; i < type_num; i++) {
            if (strcmp(types[i], p->type) == 0) {
                counts[i]++; // ����+1
                found = 1;
                break;
            }
        }
        if (!found && type_num < 100) {
            strcpy(types[type_num], p->type); // ������������
            counts[type_num] = 1;             // ��ʼ������Ϊ1
            type_num++;                    
        }
        p = p->next;
    }
    if (type_num == 0) {
        printf("    - �޶�������\n");
    }
    else { // ��ӡÿ�����͵�����
        for (int i = 0; i < type_num; i++) {
            printf("    - %s��%d��\n", types[i], counts[i]);
        }
    }
    printf("------------------------------\n");
}