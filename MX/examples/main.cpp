#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Estructura del nodo del QuadTree
struct Node {
    Node* NW; // Noroeste
    Node* NE; // Noreste
    Node* SW; // Suroeste
    Node* SE; // Sureste
    bool isLeaf;
    bool isBlack; // Representa si el nodo es negro (tiene un punto)
    int x, y; // Coordenadas del punto (si es negro)

    Node() : NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr), isLeaf(true), isBlack(false), x(0), y(0) {}
};

// Clase para representar el MX-QuadTree
class MXQuadTree {
    Node* root;
    int size;

public:
    MXQuadTree(int size) : size(size) {
        root = new Node();
    }

    void insert(int x, int y) {
        insert(root, 0, 0, size, x, y);
    }

    void exportToJson(const string& filename) {
        ofstream file(filename);
        file << "{\n";
        file << "\"name\": \"Root\",\n";
        file << "\"children\": [\n";
        exportToJson(root, file, 0, 0, size);
        file << "]\n";
        file << "}\n";
        file.close();
    }

private:
    // Inserta un punto en el árbol
    void insert(Node* node, int x_min, int y_min, int width, int x, int y) {
        if (width == 1) {
            node->isLeaf = true;
            node->isBlack = true;
            node->x = x;
            node->y = y;
            return;
        }

        if (node->isLeaf) {
            node->isLeaf = false;
            node->isBlack = false;
        }

        int half = width / 2;
        if (x < x_min + half) {
            if (y < y_min + half) {
                if (node->SW == nullptr) node->SW = new Node();
                insert(node->SW, x_min, y_min, half, x, y);
            } else {
                if (node->NW == nullptr) node->NW = new Node();
                insert(node->NW, x_min, y_min + half, half, x, y);
            }
        } else {
            if (y < y_min + half) {
                if (node->SE == nullptr) node->SE = new Node();
                insert(node->SE, x_min + half, y_min, half, x, y);
            } else {
                if (node->NE == nullptr) node->NE = new Node();
                insert(node->NE, x_min + half, y_min + half, half, x, y);
            }
        }
    }

    // Exporta el árbol al formato JSON
    void exportToJson(Node* node, ofstream& file, int x_min, int y_min, int width) {
        if (node == nullptr) {
            return;
        }

        if (node->isLeaf) {
            if (node->isBlack) {
                file << "{ \"name\": \"(" << node->x << "," << node->y << ")\" },\n";
            } else {
                file << "{ \"name\": \"Empty\" },\n";
            }
            return;
        }

        file << "{ \"name\": \"Node\",\n";
        file << "\"children\": [\n";

        exportToJson(node->SW, file, x_min, y_min, width / 2);      // SW
        exportToJson(node->NW, file, x_min, y_min + width / 2, width / 2); // NW
        exportToJson(node->SE, file, x_min + width / 2, y_min, width / 2); // SE
        exportToJson(node->NE, file, x_min + width / 2, y_min + width / 2, width / 2); // NE

        file << "]\n";
        file << "},\n";
    }
};

int main() {
    // Crear un QuadTree de tamaño 16x16
    MXQuadTree tree(16);

    // Insertar algunos puntos
    tree.insert(3, 4);
    tree.insert(10, 10);
    tree.insert(12, 14);
    tree.insert(5, 8);

    // Exportar el árbol a un archivo JSON
    tree.exportToJson("mxquadtree.json");

    cout << "Estructura exportada a mxquadtree.json" << endl;

    return 0;
}
