#include <iostream>

template<typename T>
struct nodo
{
	T t;
	nodo<T>* next;
	nodo()
	{
		next = nullptr;
	}
};

template<typename T>
struct pila{
	nodo<T>* head;
	int elem;
	
	pila()
	{
		elem = 0;
		head = nullptr;
	}
	
	void push(nodo<T>* n)
	{
		if(head == nullptr)
		{
			head = n;
		}
		else
		{
			n->next = head;
			head = n;
		}
		elem++;
	}
	void pop()
	{
		if(head != nullptr){
			nodo<T>* ctrl = head;
			if(head->next != nullptr)
				head = head->next;
			else
				head = nullptr;
			delete ctrl;
			elem--;
			}
	}
	
	T top()
	{
		return head->t;
	}
	void clear()
	{
		while (head!=nullptr){
			nodo<T>* ctrl = head;
			if(head->next != nullptr)
				head = head->next;
			else
				head = nullptr;
			delete ctrl;
			elem--;
		}
	}
	~pila()
	{
		nodo<T>* ctrl = head;
		while(ctrl != nullptr)
		{
			nodo<T>* c = ctrl;
			ctrl = ctrl->next;
			delete c;
		}	
		head = nullptr;
	}
};

template<typename T>
struct linked
{
	nodo<T>* head;
	linked()
	{
		head = nullptr;
	}
	
	void add(nodo<T>* o)
	{
		if(head== nullptr)
			head = o;
		else
		{
			nodo<T>*ctrl = head;
			while(ctrl->next != nullptr)
				ctrl = ctrl->next;
			ctrl->next = o;
		}
	}
	void add_pos(nodo<T>* o, int pos)
	{
		if(pos <= 0)
		{
			o->next = head;
			head = o;
		}
		else 
		{
			nodo<T>* ctrl = head;
			int i = 0;
			while(ctrl->next!= nullptr && i != pos-1)
			{
				ctrl = ctrl->next;
				i++;
			}
			o->next = ctrl->next;
			ctrl->next = o;
		}	
	}
	void remove(int pos)
	{
		if(pos <= 0)
		{
			nodo<T>* ctrl = head;
			nodo<T>* c = ctrl->next;
			delete ctrl;
			head = c;
		}
		else
		{
			nodo<T>* ctrl = head;
			nodo<T>* c = nullptr;
			int i = 0;
			while(ctrl->next->next!= nullptr && i != pos-1)
			{
				ctrl = ctrl->next;
				i++;
			}
			c = ctrl->next;
			ctrl->next = ctrl->next->next;
			delete c;
		}
	}
	void search(T &v)
	{
		nodo<T>* ctrl = head;
		int i = 0;
		while(ctrl!= nullptr)
		{
			if(ctrl->t == v)
				std::cout << "found at pos number "<< i << std::endl;
			i++;
			ctrl = ctrl->next;
		}
	}
	void show()
	{
		nodo<T>* ctrl = head;
		std::cout << std::endl;
		while(ctrl != nullptr)
		{
			std::cout << ctrl->t << " ";
			ctrl=ctrl->next;
		}
		std::cout << "\n";
	}
	~linked()
	{
		nodo<T>* ctrl = head;
		nodo<T>* c = nullptr;
		while(ctrl !=nullptr)
		{
			c = ctrl->next;
			delete ctrl;
			ctrl = c;
		}
	}
};


struct transicion{
	int nodo;
	char E;
	char P;
	int nodo_sig;
	std::string new_P;
};

class automata_pila{
	std::string alfabeto_E;
	std::string alfabeto_P;
	pila<char> pil;
	linked<transicion> transiciones;
	char ini;
	int * valid;
	int itera;
public:
	automata_pila()
	{
		std::cout << "ingrese alfabeto que reconocera el automata... ";
		std::cin >> alfabeto_E;
		std::cout << "ingrese alfabeto de pila... ";
		std::cin >> alfabeto_P;
		std::cout << "ingrese simbolo inicial de la pila... (alfabeto de pila '" << alfabeto_P << "')  '/' es vacio";
		std::cin>>ini;
		nodo<char> *aup = new nodo<char>();
		aup->t = ini;
		pil.push(aup); 
		std::cout << "ingrese numero de estados... ";
		std::cin >> itera;
		valid = new int[itera];
		for(int i = 0; i < itera; i++)
		{
			valid[i]= -1;
		}
		int o=0;
		for(int i  = 0; i < itera; i++)
		{
			int boo;
			std::cout<< "desea agregar transiciones con el estado " << i<<" ?... (1)si (otr)no";
			std::cin >> boo;
			while(boo==1)
			{
				nodo<transicion> *au = new nodo<transicion>();
				au->t.nodo = i;
				char au2;
				int sig;
				
				std::string cad;
				std::cout << "estado "<<i << ": ingrese simbolo de cadena";
				std::cin >> au2;
				au->t.E = au2;
				std::cout << "ingrese simbolo de pila";
				std::cin >> au2;
				au->t.P = au2;
				std::cout << "ingrese nodo siguiente... ";
				std::cin >> sig;
				au->t.nodo_sig = sig;
				std::cout << "ingrese cadena a reemplazar en pila...";
				std::cin >> cad;
				au->t.new_P = cad;
				transiciones.add(au);
				
				std::cout<< "desea agregar mas transiciones con este estado?... (1)si (otr)no";
				std::cin >> boo;
				if(boo != 1)
					break;
			}
			std::cout<< "este es un estado de validacion? (1)si (otr)no";
			std::cin >> boo;
			if(boo == 1){
				valid[o]= i;
				o++;
				}
		}
	}	
	void validar(std::string cadena)
	{
		int estado_actual = 0;
		bool iter = false;
		for(int i = 0; i < cadena.size(); i++)
		{
			nodo<transicion>* ctrl = transiciones.head;
			while(ctrl != nullptr)
			{
				if(ctrl->t.nodo == estado_actual && ctrl->t.E == cadena[i] && (ctrl->t.P == pil.top() || (ctrl->t.P == '/' && pil.elem == 0) ))
				{
					estado_actual = ctrl->t.nodo_sig;
					pil.pop();
					for(int i = ctrl->t.new_P.size()-1; i >= 0; i--){
						nodo<char>* au = new nodo<char>();
						au->t = ctrl->t.new_P[i];
						if(au->t == '/')
							delete au;
						else
							pil.push(au);
					}
					break;
				}
				ctrl=ctrl->next;
			}
			if(ctrl == nullptr)
			{
				
				if(cadena != "/"){
					iter = true;
					std::cout << "cadena invalidada... \n";}
				break;
			}
			
		}
		nodo<transicion>* ctrl = transiciones.head;
		while(ctrl != nullptr)
		{
			if(ctrl->t.nodo == estado_actual && ctrl->t.E == '/' && (ctrl->t.P == pil.top() || (ctrl->t.P == '/' && pil.elem == 0) ))
			{
				estado_actual = ctrl->t.nodo_sig;
				pil.pop();
				for(int i = ctrl->t.new_P.size()-1; i >= 0; i--){
					nodo<char>* au = new nodo<char>();
					au->t = ctrl->t.new_P[i];
					if(au->t == '/')
						delete au;
					else
						pil.push(au);
			}
			break;
			}
			ctrl=ctrl->next;
		}
	
		if(!iter)
		{
			int j = 0;
			for(int i = 0; i < itera; i++)
				if(estado_actual == valid[i])
				{
					j = 1;
					std::cout << "cadena validada...\n";
					break;
				}
			if(j == 0)
				std::cout << "cadena invalidada\n";
		}
		
		estado_actual = 0;
		pil.clear();
		nodo<char>* reset = new nodo<char>();
		reset->t = ini;
		pil.push(reset);
	}
	
	
	~automata_pila()
	{
		delete[] valid;
	}	
};

int main()
{
	automata_pila aut;
	std::string cadena;
	while(true){
		std::cout  << "ingrese cadena.. ";
		std::cin >> cadena;
		aut.validar(cadena);
	}
}
