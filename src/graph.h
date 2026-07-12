#define _CRT_SECURE_NO_WARNINGS  
#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// �߽ڵ�ṹ��
typedef struct EdgeNode {
    int end_id;          // �ߵ��յ㶥��ID
    char rel_name[50];   // �ߵĹ�ϵ����
    struct EdgeNode* next; // ָ����һ���߽ڵ��ָ��
} EdgeNode;

// ����ڵ�ṹ��
typedef struct VertexNode {
    int id;              // ����ID
    char name[50];      // ��������
    char type[30];       // ��������
    EdgeNode* first_edge; //ָ��ö���ĵ�һ���ڽӱߣ���������ͷָ�룩
    struct VertexNode* next; // ָ����һ������ڵ��ָ��
} VertexNode;

// ͼ�ṹ��
typedef struct {
    VertexNode* vertex_head; // ͷָ��
    int vertex_count;        // ��������
    int edge_count;          // ������
} Graph;

// ��������
void initGraph(Graph* g);                      // ��ʼ��ͼ�ṹ
VertexNode* findVertexById(Graph* g, int id);  // ��ID��Ų��Ҷ���
int addVertex(Graph* g, int id, char* name, char* type); // ���Ӷ���
int addEdge(Graph* g, int start_id, int end_id, char* rel_name); // ���ӱ�
void findVertexByName(Graph* g, char* name);   // �����Ʋ��Ҷ���
void findVertexByType(Graph* g, char* type);   // �����Ͳ��Ҷ���
void findVertexEdges(Graph* g, int vertex_id); // ���Ҷ������й�����
void findVertexEdgesByName(Graph* g, char* name); // �����Ʋ������
int modifyVertex(Graph* g, int id, char* new_name, char* new_type); // �޸Ķ���
int modifyEdge(Graph* g, int start_id, int end_id, char* new_rel_name); // �޸ı�
int deleteVertex(Graph* g, int id);            // ɾ�����㣨�������ߣ�
int deleteEdge(Graph* g, int start_id, int end_id); // ɾ����
void printGraphStats(Graph* g);                // ��ӡͼ��ͳ����Ϣ
#endif