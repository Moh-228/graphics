#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MAX_NODES 150
#define CELL_SIZE 100
#define PATH_WIDTH 25
#define SOLUTION_WIDTH 10
#define NODE_RADIUS 10

// Colores

typedef struct {
    int adj[MAX_NODES][MAX_NODES];
    bool visited[MAX_NODES];
    int num_nodes;
    char node_labels[MAX_NODES][4];
} Graph;

typedef struct {
    int width, height;
    int **r, **g, **b;
} Maze;

typedef struct {
    int path[MAX_NODES];
    int length;
    bool found;
} SolutionPath;

void init_graph(Graph *g, int nodes) {
    g->num_nodes = nodes;
    for(int i = 0; i < nodes; i++) {
        g->visited[i] = false;
        for(int j = 0; j < nodes; j++) {
            g->adj[i][j] = 0;
        }
    }
}

void add_edge(Graph *g, int u, int v) {
    if(u < g->num_nodes && v < g->num_nodes) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
    }
}

void create_well_distributed_tree_maze(Graph *g) {
    init_graph(g, 76);  

    add_edge(g, 0, 1);   // RAÍZ_PRINCIPAL -> TRONCO_BASE
    add_edge(g, 0, 2);   // RAÍZ_PRINCIPAL -> RAÍZ_IZQ
    add_edge(g, 0, 3);   // RAÍZ_PRINCIPAL -> RAÍZ_DER
    add_edge(g, 1, 4);   // TRONCO_BASE -> TRONCO_MEDIO
    add_edge(g, 4, 5);   // TRONCO_MEDIO -> COPA_BASE

    add_edge(g, 1, 6);   // TRONCO_BASE -> CENTRO_IZQ
    add_edge(g, 1, 7);   // TRONCO_BASE -> CENTRO_DER
    add_edge(g, 4, 8);   // TRONCO_MEDIO -> CENTRO_MEDIO_IZQ
    add_edge(g, 4, 9);   // TRONCO_MEDIO -> CENTRO_MEDIO_DER
    add_edge(g, 6, 8);   // CENTRO_IZQ -> CENTRO_MEDIO_IZQ
    add_edge(g, 7, 9);   // CENTRO_DER -> CENTRO_MEDIO_DER
    
    add_edge(g, 2, 10);  // RAÍZ_IZQ -> RAMA_IZQ_BASE
    add_edge(g, 6, 11);  // CENTRO_IZQ -> RAMA_IZQ_MEDIA
    add_edge(g, 11, 12); // RAMA_IZQ_MEDIA -> RAMA_IZQ_ALTA
    add_edge(g, 10, 13); // RAMA_IZQ_BASE -> SUB_IZQ_1
    add_edge(g, 11, 14); // RAMA_IZQ_MEDIA -> SUB_IZQ_2
    add_edge(g, 12, 15); // RAMA_IZQ_ALTA -> SUB_IZQ_3
    add_edge(g, 12, 16); // RAMA_IZQ_ALTA -> HOJA_IZQ_EXTREMA
    add_edge(g, 13, 17); // SUB_IZQ_1 -> HOJA_IZQ_1
    add_edge(g, 14, 18); // SUB_IZQ_2 -> HOJA_IZQ_2
    add_edge(g, 15, 19); // SUB_IZQ_3 -> HOJA_IZQ_3
    
    // Extensiones adicionales izquierda
    add_edge(g, 8, 20);  // CENTRO_MEDIO_IZQ -> CONECTOR_IZQ
    add_edge(g, 20, 21); // CONECTOR_IZQ -> BROTE_IZQ
    
    add_edge(g, 3, 22);  // RAÍZ_DER -> RAMA_DER_BASE
    add_edge(g, 7, 23);  // CENTRO_DER -> RAMA_DER_MEDIA
    add_edge(g, 23, 24); // RAMA_DER_MEDIA -> RAMA_DER_ALTA
    add_edge(g, 22, 25); // RAMA_DER_BASE -> SUB_DER_1
    add_edge(g, 23, 26); // RAMA_DER_MEDIA -> SUB_DER_2
    add_edge(g, 24, 27); // RAMA_DER_ALTA -> SUB_DER_3
    add_edge(g, 24, 28); // RAMA_DER_ALTA -> HOJA_DER_EXTREMA
    add_edge(g, 25, 29); // SUB_DER_1 -> HOJA_DER_1
    add_edge(g, 26, 30); // SUB_DER_2 -> HOJA_DER_2
    add_edge(g, 27, 31); // SUB_DER_3 -> HOJA_DER_3
    add_edge(g, 28, 32); // HOJA_DER_EXTREMA -> EXTENSION_DER
    
    // Extensiones adicionales derecha
    add_edge(g, 9, 33);  // CENTRO_MEDIO_DER -> CONECTOR_DER
    add_edge(g, 33, 34); // CONECTOR_DER -> BROTE_DER

    add_edge(g, 5, 35);  // COPA_BASE -> COPA_IZQ
    add_edge(g, 5, 36);  // COPA_BASE -> COPA_DER
    add_edge(g, 5, 37);  // COPA_BASE -> COPA_CENTRO
    add_edge(g, 35, 38); // COPA_IZQ -> HOJA_COPA_IZQ_1
    add_edge(g, 35, 39); // COPA_IZQ -> HOJA_COPA_IZQ_2
    add_edge(g, 36, 40); // COPA_DER -> HOJA_COPA_DER_1
    add_edge(g, 36, 41); // COPA_DER -> HOJA_COPA_DER_2
    add_edge(g, 37, 42); // COPA_CENTRO -> HOJA_CIMA_1
    add_edge(g, 37, 43); // COPA_CENTRO -> HOJA_CIMA_2
    
    
    add_edge(g, 17, 44); // HOJA_IZQ_1 -> ESQUINA_SUP_IZQ
    add_edge(g, 19, 45); // HOJA_IZQ_3 -> ESQUINA_INF_IZQ
    add_edge(g, 29, 46); // HOJA_DER_1 -> ESQUINA_INF_DER
    add_edge(g, 32, 47); // EXTENSION_DER -> ESQUINA_SUP_DER
    add_edge(g, 38, 48); // HOJA_COPA_IZQ_1 -> EXTREMO_SUP_IZQ
    add_edge(g, 40, 49); // HOJA_COPA_DER_1 -> EXTREMO_SUP_DER
    add_edge(g, 42, 50); // HOJA_CIMA_1 -> CIMA_ABSOLUTA
    add_edge(g, 21, 51); // BROTE_IZQ -> LATERAL_IZQ_EXTRA
    
    
    // Rama horizontal inferior (y=9): conectada desde nodos existentes
    add_edge(g, 17, 52); // Desde HOJA_IZQ_1 (0,9) -> (1,9)
    add_edge(g, 52, 53); // (1,9) -> (2,9)  
    add_edge(g, 53, 54); // (2,9) -> (3,9)
    add_edge(g, 54, 55); // (3,9) -> (4,9)
    add_edge(g, 55, 56); // (4,9) -> (5,9)
    add_edge(g, 56, 57); // (5,9) -> (6,9)
    add_edge(g, 57, 58); // (6,9) -> (7,9)
    
    // Rama intermedia (y=3): conectada desde RAMA_IZQ_ALTA
    add_edge(g, 12, 59); // Desde RAMA_IZQ_ALTA (1,3) -> (2,3)
    add_edge(g, 59, 60); // (2,3) -> (3,3)
    
    // Rama vertical-horizontal (x=5,6): conectada desde CENTRO_MEDIO_DER
    add_edge(g, 9, 61);  // Desde CENTRO_MEDIO_DER (6,4) -> (5,4)
    add_edge(g, 61, 62); // (5,4) -> (5,3)
    add_edge(g, 62, 63); // (5,3) -> (6,3)
    
    add_edge(g, 58, 29); // Conectar (7,9) con HOJA_DER_1 (9,9)

    // Rama vertical (x=3): conectada desde RAMA_INTERMEDIA_2
    add_edge(g, 60, 64); // Desde (3,3) -> (3,4)
    add_edge(g, 64, 65); // (3,4) -> (3,5)
    add_edge(g, 65, 66); // (3,5) -> (3,6)
    add_edge(g, 66, 67); // (3,6) -> (3,7)
    
    add_edge(g, 40, 68); // Desde HOJA_COPA_DER_1 (7,1) -> (7,2)
    add_edge(g, 68, 69); // (7,2) -> (7,3)
    add_edge(g, 69, 70); // (7,3) -> (7,4)
    
    add_edge(g, 61, 71); // Desde (5,4) -> (5,5)
    add_edge(g, 71, 72); // (5,5) -> (5,6)
    add_edge(g, 72, 73); // (5,6) -> (5,7)
    
    add_edge(g, 67, 73); // Conectar (3,7) con (5,7)
    add_edge(g, 70, 33); // Conectar (7,4) con CONECTOR_DER

}

Maze* init_maze(int grid_width, int grid_height) {
    Maze *maze = malloc(sizeof(Maze));
    maze->width = grid_width * CELL_SIZE;
    maze->height = grid_height * CELL_SIZE;
    
    
    maze->r = malloc(maze->height * sizeof(int*));
    maze->g = malloc(maze->height * sizeof(int*));
    maze->b = malloc(maze->height * sizeof(int*));
    
    for(int i = 0; i < maze->height; i++) {
        maze->r[i] = malloc(maze->width * sizeof(int));
        maze->g[i] = malloc(maze->width * sizeof(int));
        maze->b[i] = malloc(maze->width * sizeof(int));
        
        for(int j = 0; j < maze->width; j++) {
            maze->r[i][j] = 20;
            maze->g[i][j] = 20;
            maze->b[i][j] = 20;
        }
    }
    return maze;
}

void draw_circle(Maze *maze, int center_x, int center_y, int radius, int r, int g, int b) {
    for(int i = center_y - radius; i <= center_y + radius; i++) {
        for(int j = center_x - radius; j <= center_x + radius; j++) {
            if(i >= 0 && i < maze->height && j >= 0 && j < maze->width) {
                int dx = j - center_x;
                int dy = i - center_y;
                if(dx*dx + dy*dy <= radius*radius) {
                    maze->r[i][j] = r;
                    maze->g[i][j] = g;
                    maze->b[i][j] = b;
                }
            }
        }
    }
}

int abs_val(int x) {
    return x < 0 ? -x : x;
}

void draw_thick_line(Maze *maze, int x1, int y1, int x2, int y2, int thickness, int r, int g, int b) {
    int dx = abs_val(x2 - x1);
    int dy = abs_val(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    while(1) {
        draw_circle(maze, x, y, thickness/2, r, g, b);
        
        if(x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if(e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if(e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void dfs_maze(Graph *g, int node, Maze *maze, int positions[][2], int depth) {
    g->visited[node] = true;
    int node_x = positions[node][0] * CELL_SIZE + CELL_SIZE/2;
    int node_y = positions[node][1] * CELL_SIZE + CELL_SIZE/2;
    
    for(int i = 0; i < g->num_nodes; i++) {
        if(g->adj[node][i] && !g->visited[i]) {
            int next_x = positions[i][0] * CELL_SIZE + CELL_SIZE/2;
            int next_y = positions[i][1] * CELL_SIZE + CELL_SIZE/2;
            
            draw_thick_line(maze, node_x, node_y, next_x, next_y, PATH_WIDTH,250, 250, 250);
            
            dfs_maze(g, i, maze, positions, depth + 1);
        }
    }
}

void reset_visited(Graph *g) {
    for(int i = 0; i < g->num_nodes; i++) {
        g->visited[i] = false;
    }
}

bool find_path_dfs(Graph *g, int current, int target, int parent[], bool visited[]) {
    visited[current] = true;
    
    if(current == target) {
        return true;
    }
    
    for(int i = 0; i < g->num_nodes; i++) {
        if(g->adj[current][i] && !visited[i]) {
            parent[i] = current;
            if(find_path_dfs(g, i, target, parent, visited)) {
                return true;
            }
        }
    }
    
    return false;
}

SolutionPath solve_maze(Graph *g, int start_node, int target_node) {
    SolutionPath solution;
    solution.found = false;
    solution.length = 0;
    

    if(start_node < 0 || start_node >= g->num_nodes || 
       target_node < 0 || target_node >= g->num_nodes) {
        printf("Error: Nodos fuera de rango\n");
        return solution;
    }
    
    if(start_node == target_node) {
        printf("Error: Nodos idénticos\n");
        return solution;
    }
    
    int parent[MAX_NODES];
    bool visited[MAX_NODES];
    
    for(int i = 0; i < MAX_NODES; i++) {
        parent[i] = -1;
        visited[i] = false;
    }
    
    
    if(find_path_dfs(g, start_node, target_node, parent, visited)) {
        int path_stack[MAX_NODES];
        int stack_size = 0;
        int current = target_node;
        
        while(current != -1) {
            path_stack[stack_size++] = current;
            current = parent[current];
        }
        
        solution.length = stack_size;
        for(int i = 0; i < stack_size; i++) {
            solution.path[i] = path_stack[stack_size - 1 - i];
        }
        solution.found = true;
    } else {
        printf("❌ No se encontró camino\n");
    }
    
    return solution;
}

void draw_solution(Maze *maze, Graph *g, SolutionPath solution, int positions[][2]) {
    if(!solution.found) return;
 
    for(int i = 0; i < solution.length - 1; i++) {
        int from_node = solution.path[i];
        int to_node = solution.path[i + 1];
        
        int x1 = positions[from_node][0] * CELL_SIZE + CELL_SIZE/2;
        int y1 = positions[from_node][1] * CELL_SIZE + CELL_SIZE/2;
        int x2 = positions[to_node][0] * CELL_SIZE + CELL_SIZE/2;
        int y2 = positions[to_node][1] * CELL_SIZE + CELL_SIZE/2; 
        draw_thick_line(maze, x1, y1, x2, y2, SOLUTION_WIDTH, 255, 50, 50);
    }
    
}

void save_ppm(Maze *maze, const char *filename) {
    
    FILE *file = fopen(filename, "w");
    if(!file) {
        printf("Error al crear archivo\n");
        return;
    }
    
    fprintf(file, "P3\n");
    fprintf(file, "# Árbol Bien Distribuido 10x10 - DenovanMonroy - 2025-06-28 01:06:29\n");
    fprintf(file, "%d %d\n", maze->width, maze->height);
    fprintf(file, "255\n");
    
    for(int i = 0; i < maze->height; i++) {
        for(int j = 0; j < maze->width; j++) {
            fprintf(file, "%d %d %d ", maze->r[i][j], maze->g[i][j], maze->b[i][j]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Archivo guardado: %.1f MB\n", 
           (float)(maze->width * maze->height * 3) / (1024 * 1024));
}

void free_maze(Maze *maze) {
    for(int i = 0; i < maze->height; i++) {
        free(maze->r[i]);
        free(maze->g[i]);
        free(maze->b[i]);
    }
    free(maze->r);
    free(maze->g);
    free(maze->b);
    free(maze);
}

int main() {
    int USED_NODE[3]={50, 200, 50};
    Graph graph;
    create_well_distributed_tree_maze(&graph);
    
    int positions[MAX_NODES][2] = {
        // SISTEMA DE RAÍCES (bien espaciado)
        {4, 8},  // A1 - RAÍZ_PRINCIPAL (centro base)
        {4, 6},  // A2 - TRONCO_BASE
        {2, 8},  // A3 - RAÍZ_IZQ (bien separada)
        {6, 8},  // A4 - RAÍZ_DER (bien separada)
        
        // TRONCO CORTO
        {4, 4},  // A5 - TRONCO_MEDIO
        {4, 2},  // A6 - COPA_BASE
        
        // CENTRO CONECTADO PERO ESPACIADO
        {2, 6},  // A7 - CENTRO_IZQ (buena separación)
        {6, 6},  // A8 - CENTRO_DER (buena separación)
        {2, 4},  // A9 - CENTRO_MEDIO_IZQ
        {6, 4},  // A10 - CENTRO_MEDIO_DER
        
        // RAMA IZQUIERDA EXTENDIDA (bien distribuida)
        {1, 7},  // A11 - RAMA_IZQ_BASE
        {1, 5},  // A12 - RAMA_IZQ_MEDIA
        {1, 3},  // A13 - RAMA_IZQ_ALTA
        {0, 8},  // A14 - SUB_IZQ_1 (esquina)
        {0, 5},  // A15 - SUB_IZQ_2 (borde)
        {0, 3},  // A16 - SUB_IZQ_3 (borde)
        {0, 1},  // A17 - HOJA_IZQ_EXTREMA (esquina)
        {0, 9},  // A18 - HOJA_IZQ_1 (esquina)
        {0, 6},  // A19 - HOJA_IZQ_2 (borde)
        {0, 2},  // A20 - HOJA_IZQ_3 (borde)
        {1, 4},  // A21 - CONECTOR_IZQ
        {0, 4},  // A22 - BROTE_IZQ (borde)
        
        // RAMA DERECHA EXTENDIDA (bien distribuida)
        {7, 7},  // A23 - RAMA_DER_BASE
        {8, 5},  // A24 - RAMA_DER_MEDIA
        {8, 3},  // A25 - RAMA_DER_ALTA
        {9, 8},  // A26 - SUB_DER_1 (esquina)
        {9, 5},  // B1 - SUB_DER_2 (borde)
        {9, 3},  // B2 - SUB_DER_3 (borde)
        {9, 1},  // B3 - HOJA_DER_EXTREMA (esquina)
        {9, 9},  // B4 - HOJA_DER_1 (esquina)
        {9, 6},  // B5 - HOJA_DER_2 (borde)
        {9, 2},  // B6 - HOJA_DER_3 (borde)
        {9, 0},  // B7 - EXTENSION_DER (esquina)
        {7, 4},  // B8 - CONECTOR_DER
        {8, 4},  // B9 - BROTE_DER
        
        // COPA SUPERIOR DISTRIBUIDA
        {2, 2},  // B10 - COPA_IZQ (bien separada)
        {6, 2},  // B11 - COPA_DER (bien separada)
        {4, 1},  // B12 - COPA_CENTRO
        {1, 1},  // B13 - HOJA_COPA_IZQ_1 (esquina)
        {2, 0},  // B14 - HOJA_COPA_IZQ_2 (borde)
        {7, 1},  // B15 - HOJA_COPA_DER_1 (bien separada)
        {6, 0},  // B16 - HOJA_COPA_DER_2 (borde)
        {3, 0},  // B17 - HOJA_CIMA_1 (borde)
        {5, 0},  // B18 - HOJA_CIMA_2 (borde)
        
        // EXTENSIONES A ESQUINAS (bien espaciadas)
        {0, 0},  // B19 - ESQUINA_SUP_IZQ (esquina absoluta)
        {0, 7},  // B20 - ESQUINA_INF_IZQ
        {8, 8},  // B21 - ESQUINA_INF_DER
        {8, 1},  // B22 - ESQUINA_SUP_DER
        {1, 0},  // B23 - EXTREMO_SUP_IZQ (borde)
        {8, 0},  // B24 - EXTREMO_SUP_DER (borde)
        {4, 0},  // B25 - CIMA_ABSOLUTA (centro superior)
        {1, 6},  // B26 - LATERAL_IZQ_EXTRA
        
        // RAMAS ADICIONALES SOLICITADAS
        // Rama horizontal inferior (y=9)
        {1, 9},  // B27 - RAMA_HORIZONTAL_1
        {2, 9},  // B28 - RAMA_HORIZONTAL_2  
        {3, 9},  // B29 - RAMA_HORIZONTAL_3
        {4, 9},  // B30 - RAMA_HORIZONTAL_4
        {5, 9},  // B31 - RAMA_HORIZONTAL_5
        {6, 9},  // B32 - RAMA_HORIZONTAL_6
        {7, 9},  // B33 - RAMA_HORIZONTAL_7
        
        // Rama intermedia (y=3)
        {2, 3},  // B34 - RAMA_INTERMEDIA_1
        {3, 3},  // B35 - RAMA_INTERMEDIA_2
        
        // Rama vertical-horizontal
        {5, 4},  // B36 - RAMA_VERTICAL_1
        {5, 3},  // B37 - RAMA_VERTICAL_2
        {6, 3},  // B38 - RAMA_HORIZONTAL_FINAL
        
        // RAMAS ADICIONALES EXTENDIDAS
        // Rama vertical (x=3)
        {3, 4},  // B39 - RAMA_VERTICAL_3_1
        {3, 5},  // B40 - RAMA_VERTICAL_3_2
        {3, 6},  // B41 - RAMA_VERTICAL_3_3
        {3, 7},  // B42 - RAMA_VERTICAL_3_4
        
        // Rama vertical descendente (x=7)
        {7, 2},  // B43 - RAMA_VERTICAL_7_1
        {7, 3},  // B44 - RAMA_VERTICAL_7_2
        {7, 4},  // B45 - RAMA_VERTICAL_7_3
        {5, 5},  // B46 - RAMA_VERTICAL_5_1
        {5, 6},  // B47 - RAMA_VERTICAL_5_2
        {5, 7},  // B48 - RAMA_VERTICAL_5_3
    };
    
    Maze *maze = init_maze(10, 10);
    
    dfs_maze(&graph, 0, maze, positions, 0);
  
    for(int i = 0; i < graph.num_nodes; i++) {
        int x = positions[i][0] * CELL_SIZE + CELL_SIZE/2;
        int y = positions[i][1] * CELL_SIZE + CELL_SIZE/2;

        draw_circle(maze, x, y, NODE_RADIUS, USED_NODE[0], USED_NODE[1], USED_NODE[2]);
    }
    

    reset_visited(&graph);
    int start_node = 0;   // A1 - RAÍZ_PRINCIPAL
    int target_node = 30; // B19 - ESQUINA_SUP_IZQ (0,0)
    
    SolutionPath solution = solve_maze(&graph, start_node, target_node);
    
    if(solution.found) {
        draw_solution(maze, &graph, solution, positions);
        
        // Marcar nodos especiales
        int start_x = positions[start_node][0] * CELL_SIZE + CELL_SIZE/2;
        int start_y = positions[start_node][1] * CELL_SIZE + CELL_SIZE/2;
        int target_x = positions[target_node][0] * CELL_SIZE + CELL_SIZE/2;
        int target_y = positions[target_node][1] * CELL_SIZE + CELL_SIZE/2;
        
        draw_circle(maze, start_x, start_y, NODE_RADIUS + 3, 0,150,0);
        draw_circle(maze, target_x, target_y, NODE_RADIUS + 3,255, 150, 0);
    }
    
    save_ppm(maze, "Maze.ppm");
    free_maze(maze);
    


    
    return 0;
}