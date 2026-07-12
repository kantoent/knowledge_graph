﻿#define _CRT_SECURE_NO_WARNINGS
#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 边节点结构体
typedef struct EdgeNode {
    int end_id;          // 边的终点顶点ID
    char rel_name[50];   // 边的关系名称
    struct EdgeNode* next; // 指向下一个边节点的指针
} EdgeNode;

// 顶点节点结构体
typedef struct VertexNode {
    int id;              // 顶点ID
    char name[50];       // 顶点名称
    char type[30];       // 顶点类型
    EdgeNode* first_edge; // 指向该顶点的第一条邻接边（链表头指针）
    struct VertexNode* next; // 指向下一个顶点节点的指针
} VertexNode;

// 图结构体
typedef struct {
    VertexNode* vertex_head; // 头指针
    int vertex_count;        // 顶点数量
    int edge_count;          // 边数量
} Graph;

// 函数声明
void initGraph(Graph* g);                      // 初始化图结构
VertexNode* findVertexById(Graph* g, int id);  // 按编号查找顶点
int addVertex(Graph* g, int id, char* name, char* type); // 添加顶点
int addEdge(Graph* g, int start_id, int end_id, char* rel_name); // 添加边
void findVertexByName(Graph* g, char* name);   // 按名称查找顶点
void findVertexByType(Graph* g, char* type);   // 按类型查找顶点
void findVertexEdges(Graph* g, int vertex_id); // 查找顶点的所有关联边
void findVertexEdgesByName(Graph* g, char* name); // 按名称查询关联边
int modifyVertex(Graph* g, int id, char* new_name, char* new_type); // 修改顶点
int modifyEdge(Graph* g, int start_id, int end_id, char* new_rel_name); // 修改边
int deleteVertex(Graph* g, int id);            // 删除顶点（含关联边）
int deleteEdge(Graph* g, int start_id, int end_id); // 删除边
void printGraphStats(Graph* g);                // 打印图统计信息
#endif
