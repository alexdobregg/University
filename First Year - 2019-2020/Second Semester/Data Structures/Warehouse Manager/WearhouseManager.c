// Dobre Gigi-Alexandru 312 CD

#include <stdio.h>
#include <stdlib.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination) {
	// Alocam spatiul pentru structura si verificam alocarea.
	Package *package = (Package *) calloc(1, sizeof(Package));
	if(!package) {
		printf("Error allocating memory");
		exit(1);
	}
	package->priority = priority;
	/* Daca destinatia primita este egala cu NULL atunci vom seta
	si destinatia pachetului la NULL, altfel vom aloca memoria si vom
	verifica alocarea. */
	if(destination == NULL) {
		package->destination = NULL;
	} else {
		package->destination = (char *) calloc(strlen(destination) + 1, sizeof(char));
		if(!package->destination) {
			printf("Error allocating memory");
			free(package);
			exit(1);
		}
		strcpy(package->destination, destination);
	}
	return package;
}

void destroy_package(Package* package) {
	/* Daca exista pachetul, atunci vom elibera memoria
	pentru destinatie si pentru structura. */
	if(package) {
		free(package->destination);
		free(package);
	}
}

Manifest* create_manifest_node(void) {
	/* Alocam memorie pentru structura si facem verificarea alocarii
	dupa care vom seta toate campurile la NULL. */
	Manifest *manifest = (Manifest *) calloc(1, sizeof(Manifest));
	if(!manifest) {
		printf("Error allocating memory");
		exit(1);
	}
	manifest->package = NULL;
	manifest->next = NULL;
	manifest->prev = NULL;
	return manifest;
}

void destroy_manifest_node(Manifest* manifest_node) {
	/* Daca exista manifest node-ul, atunci vom folosi functia
	destroy_package in cazul in care sunt pachete si vom elibera
	memoria pentru acest node. */
	if(manifest_node) {	
		if(manifest_node->package != NULL) {
			destroy_package(manifest_node->package);
		}
		free(manifest_node);
	}
}

Wearhouse* create_wearhouse(long capacity) {
	if(capacity <= 0) {
		return NULL;
	}
	// Alocam memoria pentru wearhouse si facem verificarea acesteia.
	Wearhouse *wearhouse = (Wearhouse *) calloc(1, sizeof(Wearhouse));
	if(!wearhouse) {
		printf("Error allocating memory");
		exit(1);
	}
	// Alocam memoria pentru pachetele din wearhouse si facem verificarea acesteia.
	wearhouse->packages = (Package **) calloc(capacity, sizeof(Package *));
	if(!wearhouse->packages) {
		printf("Error allocating memory");
		free(wearhouse);
		exit(1);
	}
	wearhouse->size = 0;
	wearhouse->capacity = capacity;
	return wearhouse;
}

Wearhouse *open_wearhouse(const char* file_path) {
	size_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1) {
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1) {
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w) {
	if(w == NULL || w->size == 0) {
		return 1;
	}
	return 0;
}

long wearhouse_is_full(Wearhouse *w) {
	if(w->size == w->capacity) {
		return 1;
	}
	return 0;
}

long wearhouse_max_package_priority(Wearhouse *w) {
	if(wearhouse_is_empty(w)) {
		return 0;
	}
	int i;
	long max = 0;
	for(i = 0; i < w->size; i++) {
		if(w->packages[i]->priority > max) {
			max = w->packages[i]->priority;
		}
	}
	return max;
}

long wearhouse_min_package_priority(Wearhouse *w) {
	if(wearhouse_is_empty(w)) {
		return 0;
	}
	int i;
	long min = w->packages[0]->priority;
	for(i = 0; i < w->size; i++) {
		if(w->packages[i]->priority < min) {
			min = w->packages[i]->priority;
		}
	}
	return min;
}


void wearhouse_print_packages_info(Wearhouse *w) {
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse) {
	/* Daca exista wearhouse, vom dezaloca memoria pentru pachete
	si pentru structura. */
	if(wearhouse) {
		int i;
		for(i = 0; i < wearhouse->size; i++) {
			destroy_package(wearhouse->packages[i]);
		}
		free(wearhouse->packages);
		free(wearhouse);
	}
}


Robot* create_robot(long capacity) {
	if(capacity < 0) {
		return NULL;
	}
	// Alocam memoria, facem verificarea si setam campurile corespunzator.
	Robot *robotel = (Robot *) calloc(1, sizeof(Robot));
	if(!robotel) {
		printf("Error allocating memory");
		exit(1);
	}
	robotel->manifest =	NULL;
	robotel->next = NULL;
	robotel->size = 0;
	robotel->capacity = capacity;
	return robotel;
}

int robot_is_full(Robot* robot) {
	if(robot->size == robot->capacity) {
		return 1;
	}
	return 0;
}

int robot_is_empty(Robot* robot) {
	if(robot == NULL || robot->size == 0) {
		return 1;
	}
	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority) {
	if(wearhouse_is_empty(w) || priority < 0) {
		return NULL;
	}
	int i;
	Package *p;
	for(i = 0; i < w->size; i++) {
		if(w->packages[i]->priority == priority) {
			p = w->packages[i];
			break;
		}
	}
	return p;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package) {
	int i, poz = -1;
	if(w->size == 0) {
		return;	
	}
	// Gasim pozitia pachetului.
	for(i = 0; i < w->size; i++) {
		if(w->packages[i] == package) {
			poz = ++i;
			break;
		}
	}
	if(poz == -1) {
		return;
	}
	// Eliminam pachetul.
	for(i = poz - 1; i < w->size - 1; i++) {
		w->packages[i] = w->packages[i + 1];
	}
	w->size--;
}

void robot_load_one_package(Robot* robot, Package* package) {
	if(!robot || !package) {
		return;
	}
	if(robot_is_full(robot)) {
		return;
	}
	Manifest *newMan = create_manifest_node();
	newMan->package = package;
	// Verificam daca manifestul robotului este NULL si il actualizam daca da.
	if(robot->manifest == NULL) {
		robot->manifest = newMan;
		robot->size++;
		return;
	}
	Manifest *manIt;
	/* Iteram prin manifestul robotului pentru a gasit locul unde trebuie adaugat pachetul,
	actualizand legaturile in functie de caz, acesta putand fi introdus fie la inceput,
	fie la mijloc, fie la sfarsit, in primul rand dupa prioritate, si apoi dupa destinatie. */
	for(manIt = robot->manifest; manIt != NULL; manIt = manIt->next) {
		if(manIt->package->priority < package->priority) {
			manIt->prev = newMan;
			newMan->next = manIt;
			robot->manifest = newMan;
			break;
		} else if(manIt->package->priority == package->priority) {
			if(strcmp(manIt->package->destination, package->destination) > 0) {
				manIt->prev = newMan;
				newMan->next = manIt;
				robot->manifest = newMan;
				break;
			}
		}
		if(manIt->package->priority > package->priority) {
			if(manIt->next != NULL && manIt->next->package->priority <= package->priority) {
				newMan->prev = manIt;
				newMan->next = manIt->next;
				manIt->next->prev = newMan;
				manIt->next = newMan;
				break;
			} else if(manIt->next == NULL) {
				manIt->next = newMan;
				newMan->prev = manIt;
				break;
			}
		} else if(manIt->package->priority == package->priority &&
					strcmp(manIt->package->destination, package->destination) > 0) {
			newMan->next = manIt;
			newMan->prev = manIt->prev;
			manIt->prev->next = newMan;
			manIt->prev = newMan;
			break;
		} else if(manIt->package->priority == package->priority &&
					strcmp(manIt->package->destination, package->destination) <= 0) {
			if(manIt->next != NULL && manIt->next->package->priority == package->priority && 
					strcmp(manIt->next->package->destination, package->destination) >= 0) {
				newMan->prev = manIt;
				newMan->next = manIt->next;
				manIt->next->prev = newMan;
				manIt->next = newMan;
				break;
			} else if(manIt->next == NULL) {
				manIt->next = newMan;
				newMan->prev = manIt;
				break;
			}
		}
	}
	robot->size++;
}


long robot_load_packages(Wearhouse* wearhouse, Robot* robot) {
	if(!wearhouse || !robot) {
		return 0;
	}
	long size;
	/* Adaugam in pachetele robotului pachetul cu cea mai mare prioritate din wearhouse si
	il scoatem din acesta. */
	for(size = 0; size < robot->capacity; size++) {
		long priority = wearhouse_max_package_priority(wearhouse);
		Package *pack = robot_get_wearhouse_priority_package(wearhouse, priority);
		robot_load_one_package(robot, pack);
		robot_remove_wearhouse_package(wearhouse, pack);
	}
	robot->size = size;
	return size;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination) {
	if(!robot) {
		return NULL;
	}
	Package *p;
	Manifest *x;
	for(x = robot->manifest; x != NULL; x = x->next) {
		if(strcmp(x->package->destination, destination) == 0) {
			p = x->package;
			break;
		}
	}
	return p;
}

void destroy_robot(Robot* robot) {
	/* Daca exista robotul, parcurgem prin lista de node-uri de manifest si 
	folosim functia destroy_manifest_node dupa care eliberam memoria acestuia. */
	if(robot) {
		Manifest *robotMan = robot->manifest;
		while(robotMan != NULL) {
			Manifest *elim = robotMan;
			robotMan = robotMan->next;
			destroy_manifest_node(elim);
		}
		free(robot);
	}
}


/* Functie care face update la link-urile dintre manifest-urile robotului in functie
de pozitia manifestului, la inceput, la mijloc sau la final. */
void robot_unload_packagesHelper1(Robot *robot, Manifest *manif) {
	if(!robot || !manif) {
		return;
	}
	if(manif->prev == NULL && manif->next == NULL) {
		robot->manifest = NULL;
	} else if(manif->prev != NULL && manif->next != NULL) {
		manif->next->prev = manif->prev;
		manif->prev->next = manif->next;
		manif->next = manif->prev = NULL;
	} else if(manif->prev != NULL && manif->next == NULL) {
		manif->prev->next = NULL;
		manif->prev = NULL;
	} else if(manif->prev == NULL && manif->next != NULL) {
		manif->next->prev = NULL;
		robot->manifest = manif->next;
		manif->next = NULL;
	}
	robot->size--;
}


// Functia face update la link-urile dintre manifest-urile truck-ului.
void robot_unload_packagesHelper2(Truck *truck, Manifest *manif) {
	if(!truck || !manif) {
		return;
	}
	Manifest *truckMan;
	if(truck->manifest == NULL) {
		truck->manifest = manif;
	} else {
		truckMan = truck->manifest;
		while(truckMan->next != NULL) {
			truckMan = truckMan->next;
		}
		truckMan->next = manif;
		manif->prev = truckMan;
	}
	truck->size++;
}

void robot_unload_packages(Truck* truck, Robot* robot) {
	if(!truck || !robot) {
		return;
	}
	Manifest *roboMan = robot->manifest;
	while(robot->size > 0 && roboMan != NULL) {
		if(truck->size == truck->capacity) {
			return;
		}
		Manifest *aux = roboMan;
		roboMan = roboMan->next;
		if(strcmp(truck->destination, aux->package->destination) == 0) {
			robot_unload_packagesHelper1(robot, aux);
			robot_unload_packagesHelper2(truck, aux);
		}
	}
}


// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot) {
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time) {
	// Alocam memorie pentru structura si facem verificarea.
	Truck *truck = (Truck *) calloc(1, sizeof(Truck));
	if(!truck) {
		printf("Error allocating memory");
		exit(1);
	}
	truck->manifest = NULL;
	truck->unloading_robots = NULL;
	/* Asemanator functiei create_package, se procedeaza la fel pentru destinatie, dupa care se seteaza
	campurile corespunzator. */
	if(destination == NULL) {
		truck->destination = NULL;
	} else {
		truck->destination = (char *) calloc(strlen(destination) + 1, sizeof(char));
		if(!truck->destination) {
			printf("Error allocating memory");
			free(truck);
			exit(1);
		}
		strcpy(truck->destination, destination);
	}
	truck->capacity = capacity;
	truck->size = 0;
	truck->in_transit_time = 0;
	truck->transit_end_time = transit_time;
	truck->departure_time = departure_time;
	truck->next = NULL;
	return truck;
}

int truck_is_full(Truck *truck) {
	if(truck->size == truck->capacity) {
		return 1;
	}
	return 0;
}

int truck_is_empty(Truck *truck) {
	if(!truck || truck->size == 0) {
		return 1;
	}
	return 0;
}

long truck_destination_robots_unloading_size(Truck* truck) {
	if(!truck || !truck->unloading_robots) {
		return 0;
	}
	long size = 0;
	Robot *robotIt;
	Manifest *man;
	for(robotIt = truck->unloading_robots; robotIt != NULL; robotIt = robotIt->next) {
		for (man = robotIt->manifest; man != NULL; man = man->next) {
			if(strcmp(truck->destination, man->package->destination) == 0) {
				size = size + robotIt->size;
				break;
			}
		}
	}
	return size;
}


void truck_print_info(Truck* truck) {
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck) {
	/* Daca exista truck-ul, distrugem fiecare manifest_node si robot asociat lui,
	dupa care eliberam memoria si structura in sine. */
	if(truck) {
		Manifest *truckMan = truck->manifest;
		Robot *truckRobo = truck->unloading_robots;
		while(truckMan != NULL) {
			Manifest *elim = truckMan;
			truckMan = truckMan->next;
			destroy_manifest_node(elim);
		}
		while(truckRobo != NULL) {
			Robot *elim = truckRobo;
			truckRobo = truckRobo->next;
			destroy_robot(elim);
		}
		free(truck->destination);
		free(truck);
	}
}


Parkinglot* create_parkinglot(void) {
	// Alocam memoria pentru parkinglot si facem verificarea.
	Parkinglot *parkinglot = (Parkinglot *) calloc(1, sizeof(Parkinglot));
	if(!parkinglot) {
		printf("Error allocating memory");
		exit(1);
	}
	// Aloca memoria pentru santinele si facem verificarile.
	parkinglot->arrived_trucks = (Truck *) calloc(1, sizeof(Truck));
	if(!parkinglot->arrived_trucks) {
		printf("Error allocating memory");
		free(parkinglot);
		exit(1);
	}
	parkinglot->departed_trucks = (Truck *) calloc(1, sizeof(Truck));
	if(!parkinglot->departed_trucks) {
		printf("Error allocating memory");
		free(parkinglot->arrived_trucks);
		free(parkinglot);
		exit(1);
	}
	parkinglot->pending_robots = (Robot *) calloc(1, sizeof(Robot));
	if(!parkinglot->pending_robots) {
		printf("Error allocating memory");
		free(parkinglot->arrived_trucks);
		free(parkinglot->departed_trucks);
		free(parkinglot);
		exit(1);
	}
	parkinglot->standby_robots = (Robot *) calloc(1, sizeof(Robot));
	if(!parkinglot->standby_robots) {
		printf("Error allocating memory");
		free(parkinglot->arrived_trucks);
		free(parkinglot->departed_trucks);
		free(parkinglot->pending_robots);
		free(parkinglot);
		exit(1);
	}
	// Setam next-ul santinelelor la ele.
	parkinglot->arrived_trucks->next = parkinglot->arrived_trucks;
	parkinglot->departed_trucks->next = parkinglot->departed_trucks;
	parkinglot->pending_robots->next = parkinglot->pending_robots;
	parkinglot->standby_robots->next = parkinglot->standby_robots;

	return parkinglot;
}

Parkinglot* open_parckinglot(const char* file_path) {
	size_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot) {
	if(!parkinglot || !robot) {
		return;
	}
	/* Verificam daca robotul trebuie adaugat in lista de standby sau in cea de pending,
	dupa care iteram prin aceasta lista si gasim locul in care trebuie adaugat. */
	if(robot->size == 0) {
		Robot *iter = parkinglot->standby_robots->next;
		Robot *prev = parkinglot->standby_robots;
		if(parkinglot->standby_robots == iter) {
			robot->next = parkinglot->standby_robots;
			parkinglot->standby_robots->next = robot;
		} else {
			while(iter != parkinglot->standby_robots && iter->capacity > robot->capacity) {
				prev = iter;
				iter = iter->next;
			} 
			prev->next = robot;
			robot->next = iter;
		}
	} else {
		Robot *iter = parkinglot->pending_robots->next;
		Robot *prev = parkinglot->pending_robots;
		if(parkinglot->pending_robots == parkinglot->pending_robots->next) {
			robot->next = parkinglot->pending_robots;
			parkinglot->pending_robots->next = robot;
		} else {
			while(iter != parkinglot->pending_robots && iter->size > robot->size) {
				prev = iter;
				iter = iter->next;
			}
			prev->next = robot;
			robot->next = iter;
		}
	}
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot) {
	if(!parkinglot || !robot) {
		return;
	}
	/* Verificam daca robotul trebuie eliminat din lista de standby sau din cea de pending,
	si facem update-ul necesar link-urilor. */
	if(robot->size == 0) {
		if(parkinglot->standby_robots == parkinglot->standby_robots->next) {
			return;
		}
		Robot *iter = parkinglot->standby_robots->next;
		Robot *prev = parkinglot->standby_robots;
		while(iter != parkinglot->standby_robots) {
			if(iter == robot) {
				prev->next = iter->next;
				break;
			}
			prev = iter;
			iter = iter->next;
		}
	} else {
		if(parkinglot->pending_robots == parkinglot->pending_robots->next) {
			return;
		}
		Robot *iter = parkinglot->pending_robots->next;
		Robot *prev = parkinglot->pending_robots;
		while(iter != parkinglot->pending_robots) {
			if(iter == robot) {
				prev->next = iter->next;
				break;
			}
			prev = iter;
			iter = iter->next;
		}
	}
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot) {
	if(!parkinglot ) {
		return 0;
	}
	if(parkinglot->pending_robots == parkinglot->pending_robots->next) {
		return 0;
	}
	return 1;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot) {
	if(!parkinglot) {
		return 0;
	}
	Truck *truck = parkinglot->arrived_trucks->next;
	while(truck != parkinglot->arrived_trucks) {
		if(truck->manifest != NULL) {
			return 0;
		}
		truck = truck->next;
	}
	return 1;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot) {
	if(!parkinglot) {
		return 0;
	}
	if(parkinglot->departed_trucks == parkinglot->departed_trucks->next) {
		return 0;
	}
	return 1;
}


void destroy_parkinglot(Parkinglot* parkinglot) {
	Robot *iterSBR = parkinglot->standby_robots->next;
	Robot *iterPR = parkinglot->pending_robots->next;
	Truck *iterAT = parkinglot->arrived_trucks->next;
	Truck *iterDT = parkinglot->departed_trucks->next;
	/* Se parcurge prin fiecare lista si se folosesc functiile necesare
	distrugerii node-urilor, dupa care se distrug si santinelele si se
	elibereaza memoria structurii in sine. */
	while(iterSBR != parkinglot->standby_robots) {
		Robot *elimSBR = iterSBR;
		iterSBR = iterSBR->next;
		destroy_robot(elimSBR);
	}
	while(iterPR != parkinglot->pending_robots) {
		Robot *elimPR = iterPR;
		iterPR = iterPR->next;
		destroy_robot(elimPR);
	}
	while(iterAT != parkinglot->arrived_trucks) {
		Truck *elimAT = iterAT;
		iterAT = iterAT->next;
		destroy_truck(elimAT);
	}
	while(iterDT != parkinglot->departed_trucks) {
		Truck *elimDT = iterDT;
		iterDT = iterDT->next;
		destroy_truck(elimDT);
	}

	destroy_robot(parkinglot->standby_robots);
	destroy_robot(parkinglot->pending_robots);
	destroy_truck(parkinglot->arrived_trucks);
	destroy_truck(parkinglot->departed_trucks);

	free(parkinglot);
}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}



void truck_departed(Parkinglot *parkinglot, Truck* truck){
	if(!parkinglot || !truck) {
		return;
	}
	// Se verifica daca truck-ul se afla in lista de arrived si se elimina daca da.
	Truck *truckEl, *truckPrv;
	if(parkinglot->arrived_trucks != parkinglot->arrived_trucks->next) {
		truckEl = parkinglot->arrived_trucks->next;
		truckPrv = parkinglot->arrived_trucks;
		while(truckEl != parkinglot->arrived_trucks) {
			if(truckEl == truck) {
				truckPrv->next = truckEl->next;
				break;
			}
			truckPrv = truckEl;
			truckEl = truckEl->next;
		}
	}
	/* Se itereaza prin lista de departed si se fac update-urile necesare
	introducerii lui in aceasta. */
	Truck *truckIt;
	Truck *truckPrv2;
	if(parkinglot->departed_trucks == parkinglot->departed_trucks->next) {
		truck->next = parkinglot->departed_trucks;
		parkinglot->departed_trucks->next = truck;
	} else {
		truckIt = parkinglot->departed_trucks->next;
		truckPrv2 = parkinglot->departed_trucks;
		while(truckIt != parkinglot->departed_trucks && truckIt->departure_time
													< truck->departure_time) {		
			truckPrv2 = truckIt;
			truckIt = truckIt->next;
		}
		truckPrv2->next = truck;
		truck->next = truckIt;
	}
}


void truck_arrived(Parkinglot *parkinglot, Truck* truck) {
	if(!parkinglot || !truck) {
		return;
	}
	// Se verifica daca face parte din lista de departed si se elimina din aceasta daca da.
	Truck *truckEl, *truckPrv;
	if(parkinglot->departed_trucks != parkinglot->departed_trucks->next) {
		truckEl = parkinglot->departed_trucks->next;
		truckPrv = parkinglot->departed_trucks;
		while(truckEl != parkinglot->departed_trucks) {
			if(truckEl == truck) {
				truckPrv->next = truckEl->next;
				break;
			}
			truckPrv = truckEl;
			truckEl = truckEl->next;
		}
	}
	// Se goleste manifestul truck-ului si se seteaza la NULL.
	Manifest *truckMan = truck->manifest;
	while(truckMan != NULL) {
		Manifest *aux = truckMan;
		truckMan = truckMan->next;
		destroy_manifest_node(aux);
	}
	truck->manifest = NULL;
	Truck *truckIt, *truckPrv2;
	// Se itereaza prin lista de arrived si se introduce la locul corespunzator.
	if(parkinglot->arrived_trucks == parkinglot->arrived_trucks->next) {
		truck->size = 0;
		truck->in_transit_time = 0;
		truck->next = parkinglot->arrived_trucks;
		parkinglot->arrived_trucks->next = truck;
	} else {
		truckIt = parkinglot->arrived_trucks->next;
		truckPrv2 = parkinglot->arrived_trucks;
		while(truckIt != parkinglot->arrived_trucks &&
				strcmp(truckIt->destination, truck->destination) < 0) {
			truckPrv2 = truckIt;
			truckIt = truckIt->next;
		}
		while(truckIt != parkinglot->arrived_trucks &&
				truckIt->departure_time < truck->departure_time) {
			if(strcmp(truckIt->destination, truck->destination) != 0) {
				break;
			}
			truckPrv2 = truckIt;
			truckIt = truckIt->next;
		}
		// Se seteaza corespunzator.
		truck->size = 0;
		truck->in_transit_time = 0;
		truckPrv2->next = truck;
		truck->next = truckIt;
	}
}	


/* */

void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck) {
	if(!parkinglot || !truck) {
		return;
	}
	Robot *robotIt;
	robotIt = truck->unloading_robots;
	// Se muta robotii unui truck intr-una din listele parkinglot-ului de roboti.
	while(robotIt != NULL) {
		Robot *robotAD = robotIt;
		robotIt = robotIt->next;
		parkinglot_add_robot(parkinglot, robotAD);
	}
	truck->unloading_robots = NULL;
}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour) {
	if(!parkinglot) {
		return;
	}
	Truck *truckIt = parkinglot->arrived_trucks->next;
	/* Se itereaza prin lista de arrived si se transfera robotii intr-una
	din listele parkinglot-ului de roboti si se adauga truck-ul in lista de departed */
	while(truckIt != parkinglot->arrived_trucks) {
		Truck *truckUp = truckIt;
		truckIt = truckIt->next;
		if(truckUp->departure_time == day_hour) {
			truck_transfer_unloading_robots(parkinglot, truckUp);
			truck_departed(parkinglot, truckUp);
		}
	}
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot) {
	if(!parkinglot) {
		return;
	}
	/* Se itereaza prin lista de departed si se adauga in arrived
	daca in_transit_time este egal cu transit_end_time. */
	Truck *truckIt = parkinglot->departed_trucks->next;
	while(truckIt != parkinglot->departed_trucks) {
		Truck *truckUp = truckIt;
		truckIt = truckIt->next;
		truckUp->in_transit_time += 1;
		if(truckUp->in_transit_time == truckUp->transit_end_time) {
			truck_arrived(parkinglot, truckUp);
		}
	}
}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

