# Knowledge Graph / 知识图谱系统

> 数据结构课程实验项目 —— 基于邻接表 + SQLite 的图存储与查询系统  
> Data Structures Lab Project — Graph storage & query system using adjacency list and SQLite

## 📋 项目简介 | Introduction

本项目实现了一个**有向图知识图谱**系统，支持：

- 图的**邻接表**存储结构（链式存储）
- 顶点和边的**增删改查**操作
- 按名称/类型**模糊搜索**顶点
- 查询顶点的所有关联边
- SQLite 持久化存储（保存/加载）
- 图结构**遍历**与**统计**分析

Built as a Data Structures course project, implementing a directed knowledge graph with adjacency list, CRUD operations, fuzzy search, and SQLite persistence.

## 🧱 项目结构 | Project Structure

```
knowledge_graph/
├── src/                    # 源代码 Source code
│   ├── graph.h             # 图结构定义（邻接表）
│   ├── graph.c             # 图操作实现
│   ├── knowledge_graph.c   # 主程序入口 & 菜单界面
│   ├── sqlite3.h           # SQLite 头文件
│   └── sqlite3.c           # SQLite 数据库引擎
├── data/                   # 数据库文件目录
│   └── ds_graph.db         # SQLite 数据文件
├── knowledge_graph.sln     # Visual Studio 解决方案
├── knowledge_graph.vcxproj # 项目文件
├── .gitignore              # Git 忽略规则
└── README.md               # 本文件
```

## 🚀 构建与运行 | Build & Run

### Windows (Visual Studio 2022)

1. 双击打开 `knowledge_graph.sln`
2. 按 `F5` 或点击 **本地 Windows 调试器** 运行
3. 或：在菜单栏选择 **生成 → 生成解决方案** (`Ctrl+Shift+B`)

### 命令行 (MSBuild)

```bash
msbuild knowledge_graph.sln /p:Configuration=Debug /p:Platform=x64
```

## 🎮 功能菜单 | Feature Menu

| 编号 | 功能               | Description                      |
|------|--------------------|----------------------------------|
| 1    | 添加顶点           | Add vertex                       |
| 2    | 添加边             | Add edge                         |
| 3    | 查询顶点           | Query vertex (by ID/name/type)   |
| 4    | 修改顶点           | Modify vertex                    |
| 5    | 修改边             | Modify edge                      |
| 6    | 删除顶点           | Delete vertex                    |
| 7    | 删除边             | Delete edge                      |
| 8    | 浏览图             | Traverse / display graph         |
| 9    | 保存到数据库       | Save to SQLite                   |
| 10   | 从数据库加载       | Load from SQLite                 |
| 11   | 图统计             | Graph statistics                 |
| 12   | 清屏               | Clear screen                     |
| 0    | 退出               | Exit                             |

## 🛠️ 技术要点 | Technical Highlights

- **存储结构**：邻接表（链表）存储有向图
- **顶点节点**：`VertexNode` (id, name, type, first_edge, next)
- **边节点**：`EdgeNode` (end_id, rel_name, next)
- **持久化**：SQLite 通过 `sqlite3_exec` / `sqlite3_prepare_v2` 实现 CRUD
- **编码**：UTF-8 without BOM

## 📚 课程信息 | Course Info

- **课程**：数据结构 (Data Structures)
- **项目类型**：课程设计实验 (Lab Project)
- **主题**：图的存储与操作 (Graph Storage & Operations)

## 📄 License

MIT
