#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Segmento {
  int direccion;
  int size;
  bool status;
  std::vector<int> procesosAsignados;
  // constructor
  Segmento(int direccion, int size, bool status)
      : direccion(direccion), size(size), status(status) {}

  // Método para asignar un proceso a este segmento
  void asignarProceso(int procesoID) { procesosAsignados.push_back(procesoID); }

  // Método para liberar un proceso de este segmento
  void liberarProceso(int procesoID) {
    auto it = std::find(procesosAsignados.begin(), procesosAsignados.end(),
                        procesoID);
    if (it != procesosAsignados.end()) {
      procesosAsignados.erase(it);
    }
  }
};

struct Proceso {
  int id;
  int sizeProceso;

  // Constructor
  Proceso(int id, int sizeProceso) : id(id), sizeProceso(sizeProceso) {}
};

class memoriaSegmentada {
private:
  std::vector<Segmento> segmentos;

public:
  memoriaSegmentada(int sizeMemoria, int sizeSegmento) {
    int numSegmentos = sizeMemoria / sizeSegmento;
    for (int i = 0; i < numSegmentos; i++) {
      segmentos.emplace_back(i * sizeSegmento, sizeSegmento, false);
    }
  }

  // asignar memoria a un proceso
  bool asignarMemoria(Proceso &proceso) {
    for (auto &segmento : segmentos) {
      if (!segmento.status && segmento.size >= proceso.sizeProceso) {
        segmento.size -= proceso.sizeProceso;
        if (segmento.size <= 0) {
          segmento.status = true;
        }
        segmento.asignarProceso(proceso.id);
        std::cout << "Proceso " << proceso.id << " asignado en segmento "
                  << segmento.direccion << std::endl;
        return true;
      }
    }
    std::cout << "No hay memoria suficiente para el proceso " << proceso.id
              << std::endl;
    return false;
  }

  // liberar memoria de un proceso
  void liberarMemoria(int direccionSegmento) {
    for (auto &segmento : segmentos) {
        segmento.liberarProceso(direccionSegmento);
        if (segmento.procesosAsignados.empty()) {
          segmento.status = false;
          std::cout << "Segmento " << segmento.direccion << " liberado"
                    << std::endl;
        }
      }
    return;
  }

  // imprimir estado de la memoria
  void imprimirStatusMem() const {
    std::cout << "Estado de la memoria segmentada:" << std::endl;
    for (const auto &segmento : segmentos) {
      std::cout << "Dirección: " << segmento.direccion
                << ", Tamaño: " << segmento.size
                << ", Estado: " << (segmento.status ? "Ocupado" : "Libre")
                << ", Procesos asignados: [";
      for (int procesoID : segmento.procesosAsignados) {
        std::cout << procesoID << " ";
      }
      std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
  }
};

class SimuladorSegmentacion {
private:
  int tamanoMemoria;
  int tamanoSegmento;
  memoriaSegmentada memoriaSegmentada;
  std::vector<Proceso> procesos;

public:
  SimuladorSegmentacion(int tamanoMemoria, int tamanoSegmento)
      : tamanoMemoria(tamanoMemoria), tamanoSegmento(tamanoSegmento),
        memoriaSegmentada(tamanoMemoria, tamanoSegmento){};

  // Método para asignar memoria a un proceso
  bool asignarMemoria(int procesoID, int tamano) {
    Proceso proceso(procesoID, tamano);
    if (memoriaSegmentada.asignarMemoria(proceso)) {
      procesos.push_back(proceso);
      return true;
    }
    return false;
  }

  // Imrpimir estado:
  void printStatus() const { memoriaSegmentada.imprimirStatusMem(); }

  // Liberar espacio en memoria
  void liberarMemoria(int procesoID) {
    memoriaSegmentada.liberarMemoria(procesoID);
    // eliminar el proceso de los procesos
    procesos.erase(std::remove_if(procesos.begin(), procesos.end(),
                                  [procesoID](const Proceso &proceso) {
                                    return proceso.id == procesoID;
                                  }),
                   procesos.end());
  }
};

int main() {
  SimuladorSegmentacion simulador(300, 100);

  simulador.asignarMemoria(1, 120);
  simulador.asignarMemoria(2, 30);
  simulador.asignarMemoria(3, 40);
  simulador.asignarMemoria(4, 70);
  simulador.asignarMemoria(5, 100);

  // Liberar memoria de un proceso
  simulador.printStatus();
  simulador.liberarMemoria(2);
  simulador.liberarMemoria(3);
  simulador.printStatus();

  // agregar un proceso con memoria más grande de la disponible
  simulador.asignarMemoria(3, 200);
}