#pragma once
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

struct Punto {
    double x;
    double y;
};

inline double DistanciaEucladiana(const Punto& a, const Punto& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

struct NodoArbolKD {
    Punto punto;       
    int index;
    NodoArbolKD* left;    
    NodoArbolKD* right;

    NodoArbolKD(const Punto& p, int idx) : punto(p), index(idx), left(nullptr), right(nullptr) {}
};

class ArbolKD {
	public:
	    ArbolKD() : root(nullptr) {}

	    ~ArbolKD() { destroy(root); }

	    void build(const std::vector<Punto>& puntos) {
	        destroy(root);
	        root = nullptr;

	        std::vector<std::pair<Punto,int>> pts;
	        pts.reserve(puntos.size());
	        for (int i = 0; i < (int)puntos.size(); i++)
	            pts.push_back({puntos[i], i});

	        root = buildRec(pts, 0, (int)pts.size() - 1, 0);
	    }
	     
	    int VecinoCercano(const Punto& query) const {
	        double mejorDistancia = std::numeric_limits<double>::max();
	        int mejorIndice = -1;
	        busquedaNN(root, query, 0, mejorDistancia, mejorIndice);
	        return mejorIndice;
	    }

	private:
	    NodoArbolKD* root;
	   	NodoArbolKD* buildRec(std::vector<std::pair<Punto,int>>& pts, int lo, int hi, int depth) {
	        if (lo > hi) {
	        	return nullptr;
			}
			
	        int axis = depth % 2;
	        std::sort(pts.begin() + lo, pts.begin() + hi + 1, [axis](const std::pair<Punto,int>& a, const std::pair<Punto,int>& b) { return axis == 0 ? a.first.x < b.first.x : a.first.y < b.first.y;});
	        int mid = (lo + hi) / 2;
	        NodoArbolKD* nodo = new NodoArbolKD(pts[mid].first, pts[mid].second);

	        nodo->left  = buildRec(pts, lo, mid - 1, depth + 1);
	        nodo->right = buildRec(pts, mid + 1, hi, depth + 1);
	        return nodo;
	    }

	    void busquedaNN(NodoArbolKD* nodo, const Punto& query, int depth, double& mejorDistancia, int& mejorIndice) const {
	        if (nodo == nullptr) {
	        	return;
			}
			
	        double d = DistanciaEucladiana(query, nodo->punto);
	        if (d < mejorDistancia) {
	            mejorDistancia = d;
	            mejorIndice = nodo->index;
	        }

			int axis = depth % 2;
	        double diff = (axis == 0) ? (query.x - nodo->punto.x) : (query.y - nodo->punto.y);

	        NodoArbolKD* first = (diff <= 0) ? nodo->left : nodo->right;
	        NodoArbolKD* second = (diff <= 0) ? nodo->right : nodo->left;

	        busquedaNN(first, query, depth + 1, mejorDistancia, mejorIndice);

	        if (diff * diff < mejorDistancia) {
	            busquedaNN(second, query, depth + 1, mejorDistancia, mejorIndice);
	        }
	    }

	    void destroy(NodoArbolKD* nodo) {
	        if (!nodo) {
	        	return;
	        }
	        
	        destroy(nodo->left);
	        destroy(nodo->right);
	        delete nodo;
	    }
};
