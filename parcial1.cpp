#include <iostream>  
#include <fstream>  
#include <string> 
#include <vector>
#include <sstream>  
#include <queue>

using namespace std;

struct Process{  
    int Pid;        	
    int AT; 			
    int BT;   			
    int PQ;    			
    int WT; 			
    int RT; 			
    int CT;  			
};


std::vector<Process> leerEntrada(const std::string &archivo){
	std::ifstream inputFile(archivo);
	if (!inputFile) {
		std::cerr << "Error: No se pudo abrir el archivo " << std::endl;
		return {};
	}
	
	std::vector<Process> procesos;  
    std::string linea;   
    while (std::getline(inputFile, linea)) {  
        std::istringstream iss(linea);  
        Process p;  
        if (!(iss >> p.Pid >> p.AT >> p.BT >> p.PQ)) {  
            std::cerr << "Error al leer la linea: " << linea << std::endl;  
            continue; 
        }
        p.WT = 0;       
        p.RT = -1;     
        p.CT = 0;      
        procesos.push_back(p);
    }  
    inputFile.close();
    return procesos;
};


//Funcion para el algoritmo de FCFS
int FCFS(Process &proceso, int &time){
	if (proceso.RT == -1) {  
        proceso.RT = time; // Establecer tiempo de inicio  
    } 
    
	time += proceso.BT;
    proceso.CT = time;
    proceso.WT = time - proceso.AT - proceso.BT;
    proceso.BT = 0;
    return time;
};


//Funcion para el algoritmo de RR
int RR(Process &proceso, int quantum, int &time){
	if (proceso.BT > 0) {
		if (proceso.RT == -1) {  
		    proceso.RT = time; // Establecer tiempo de inicio  
		} 
		
		if (proceso.BT > quantum) {
		    time += quantum;
		    proceso.BT -= quantum;
		}
		else{
			time += proceso.BT;
		    proceso.CT = time;
		    proceso.WT = time - proceso.AT - proceso.BT;
		    proceso.BT = 0;
		}
		return time;
	}
};

//Funcion para el algoritmo de MLQ
void MLQ(std::vector<Process> &procesos, int quantum){
	std::queue<Process*> colaRR;   
    std::queue<Process*> colaFCFS; 
    int time = 0;
    
    while (true) {  
        // Agregar procesos a las colas segun su tiempo de llegada  
        for (auto &p : procesos) {  
            if (p.AT <= time && p.BT > 0) {  
                if (p.PQ == 1) {  
                    colaRR.push(&p);  
                } else if (p.PQ == 2) {  
                    colaFCFS.push(&p);  
                }  
            }  
        } 
        
    	if (!colaRR.empty()){
    		Process* currentProcess = colaRR.front();  
            colaRR.pop();
		    
            RR(*currentProcess, quantum, time); // Llamar a la funcion RR  
            continue;
		}
		
		if (!colaFCFS.empty()) {  
            Process* currentProcess = colaFCFS.front();  
            colaFCFS.pop();  
            FCFS(*currentProcess, time); // Llamar a la funcion FCFS  
        }
        
        // Verificar si todos los procesos han terminado  
        bool allDone = true;  
        for (const auto &p : procesos) {  
            if (p.BT > 0) {  
                allDone = false;  
                break;  
            }  
        }
        if (allDone) break;
    }
};

//Funcion para el algoritmo de MLFQ
int MLFQ(){
	
};

int main(){
	std::vector<Process> procesos = leerEntrada("entrada.txt");
	
	int quantum = 3; // Tiempo de quantum para RR  
    MLQ(procesos, quantum);
    
    std::cout << "MLQ\n";
    std::cout << "PID\tWT\tRT\tCT\n";  
    for (const auto &proceso : procesos) {  
        std::cout << "P"<< proceso.Pid << "\t"   
                  << proceso.WT << "\t"   
                  << proceso.RT << "\t"   
                  << proceso.CT << std::endl;  
    } 
    
    double totalWT = 0, totalRT = 0;  
    for (const auto &proceso : procesos) {  
        totalWT += proceso.WT;  
        totalRT += proceso.RT;  
    }
    double avgWT = totalWT / procesos.size();  
    double avgRT = totalRT / procesos.size();
    
    std::cout << "Tiempo de espera promedio (WT): " << avgWT << std::endl;  
    std::cout << "Tiempo de respuesta promedio (RT): " << avgRT << std::endl;  

    return 0;
	
};
