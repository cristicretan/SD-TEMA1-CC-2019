#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct nod
{
	int timestamp;
	double value;
	struct nod *prev;
	struct nod *next;
} *Node;


// functie de inserare in lista la o anumita pozitie
void insertNode(Node *head, int timestamp, double value, int pos) {
	int k = 1;
	Node nou = (Node)malloc(sizeof(struct nod));
	if(!nou) {
		fprintf(stderr, "Memory was not correctly allocated.\n");
		exit(1);
	}
	nou->value = value;
	nou->timestamp = timestamp;
	Node p = *head;
	if(p == NULL) {
		nou->next = NULL;
		nou->prev = NULL;
		*head = nou;
		return;
	}
	if(p != NULL && pos == 1) {
		nou->next = *head;
		(*head)->prev = nou;
		nou->prev = NULL;
		*head = nou;
		return;
	}
	while(p->next != NULL && k < pos) {
		k++;
		p = p->next;
	}
	if(p->next == NULL) {
		p->next = nou;
		nou->prev = p;
		nou->next = NULL;
	}
	else if(p->next != NULL) {
		p->next->prev = nou;
		nou->next = p->next;
		nou->prev = p;
		p->next = nou;
	}
}

// functie de stergere a unei anumite valori din lista
void deleteNode(Node *head, double value) {
	Node p = *head;
	if(p == NULL) return;
	if(p->value == value) {
		Node q = p;
		p = p->next;
		p->prev = NULL;
		*head = p;
		free(q);
		return;
	}
	while(p != NULL) {
		if(p->value == value) {
			Node q = p;
			p->prev->next = p->next;
			if(p->next != NULL) p->next->prev = p->prev;
			free(q);
			break;
		}
		p = p->next;
	}
}

// functie de stergere a listei si eliberare a memoriei
void freeList(Node head) {
	Node p = head;
	while(p != NULL) {
		Node q = p;
		p = p->next;
		free(q);
	}
}

// functie de printare a listei cu aproximatie de 2 zecimale
void print(Node head) {
	while(head != NULL) {
		printf("%d %.2lf\n", head->timestamp, head->value);
		head = head->next;
	}
}

// Task 1.1
// Trebuie eliminate exceptiile din intrari adica
// basically sa stergi toate valorile care nu se afla intr-un 
// anumit interval dat de average-ul si deviatia 
// unei ferestre de K noduri
// De asemenea, am facut toate cerintele pentru un K arbitrar nu am hardcodat
// nici una specificatii.
void eliminareExceptii(Node *head, int *n)
{
	double xmediu, deviatie;
	int k = 5;
	int start = 0;
	int end = k;
	Node p = *head;
	Node del = NULL;
	int nr = 0;
	// imi iau o lista noua denumita del un pointer catre capul listei
	// un start si un end care sa simbolizeze inceputul si 
	// sfarsitul ferestrei din lista
	// si aplic efectiv formulele din task
	while(end <= *n)
	{
		Node q = p;
		int i = start;
		xmediu = 0;
		deviatie = 0;
		while(q != NULL && i < end)
		{
			xmediu += q->value;
			i++;
			q = q->next;
		}
		xmediu /= k;
		q = p;
		i = start;
		while(q != NULL && i < end)
		{
			deviatie += (q->value - xmediu)*(q->value - xmediu);
			i++;
			q = q->next;
		}
		deviatie /= k;
		deviatie = sqrt(deviatie);

		q = p;
		i = start;
		while(q != NULL && i < end)
		{
			if(i >= start + k/2 && i < end - k/2)
			{
				if(q->value < xmediu - deviatie || q->value > xmediu + deviatie)
				{
					insertNode(&del, q->timestamp, q->value, nr + 1);
					nr++;
				}
			}
			i++;
			q = q->next;
		}
		p = p->next;
		start++;
		end++;
	}
	p = del;
	// fac pointer-ul p sa fie la inceputul vectorului
	// de noduri care trebuie eliminate
	// dupa care le elimin pe fiecare din lista initiala
	while(p != NULL)
	{
		deleteNode(head, p->value);
		(*n)--;
		p = p->next;
	}
	// eliberez memoria listei nou definite
	freeList(del);
}

// functie de gasire a nodului din mijloc a unei 
// liste simplu/dublu inlantuite
Node findMiddle(Node head)
{
	Node slowptr = head;
	Node fastptr = head;
	while(fastptr->next != NULL && fastptr->next->next != NULL)
	{
		fastptr = fastptr->next->next;
		slowptr = slowptr->next;
	}
	return slowptr;
}

// functie auxiliara pentru Mergesort
Node Split(Node head)
{
	Node fast = head;
	Node slow = head;
	while(fast->next && fast->next->next)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	Node temp = slow->next;
	slow->next = NULL;
	return temp;
}

// implementez merge sort pt valori 
// cat si pt timestamp-uri daca o sa fie necesar
Node MergeVal(Node first, Node second)
{
	if(!first) return second;
	if(!second) return first;

	if(first->value < second->value)
	{
		first->next = MergeVal(first->next, second);
		first->next->prev = first;
		first->prev = NULL;
		return first;
	}
	else
	{
		second->next = MergeVal(first, second->next);
		second->next->prev = second;
		second->prev = NULL;
		return second;
	}
}

Node MergeSortVal(Node head)
{
	if(!head || !head->next) return head;
	Node second = Split(head);

	head = MergeSortVal(head);
	second = MergeSortVal(second);

	return MergeVal(head, second);
}

Node MergeTime(Node first, Node second)
{
	if(!first) return second;
	if(!second) return first;

	if(first->timestamp < second->timestamp)
	{
		first->next = MergeTime(first->next, second);
		first->next->prev = first;
		first->prev = NULL;
		return first;
	}
	else
	{
		second->next = MergeTime(first, second->next);
		second->next->prev = second;
		second->prev = NULL;
		return second;
	}
}

Node MergeSortTime(Node head)
{
	if(!head || !head->next) return head;
	Node second = Split(head);

	head = MergeSortTime(head);
	second = MergeSortTime(second);

	return MergeTime(head, second);
}

// functie de creare a unei subliste
// folosita ca fereastra de K noduri
Node createSublist(Node head, int start, int end)
{
	Node p = head;
	Node res = NULL;
	int i = 0;
	while(p != NULL && i < start)
	{
		i++;
		p = p->next;
	}
	int k = 0;
	while(p != NULL && i < end)
	{
		insertNode(&res, p->timestamp, p->value, k + 1);
		i++;
		k++;
		p = p->next;
	}
	return res;
}

// Task 1.2.1
// filtrarea mediana
Node filtrareMediana(Node head, int n, int *nr)
{
	int k = 5;
	int i = 0;
	int start = 0;
	int end = k;
	*nr = 0;
	Node nou = NULL;
	// la fel, imi iau un start si un end
	// reprezentand o fereastra de lungime K
	// apoi implementez cerinta
	// sortand fereastra de lungime K
	// luand elementul median
	// si il pun in lista nou definita
	while(end <= n)
	{
		Node sortedval = createSublist(head, start, end);
		Node sortedtime = createSublist(head, start, end);
		sortedval = MergeSortVal(sortedval);
		sortedtime = MergeSortTime(sortedtime);
		Node midval = findMiddle(sortedval);
		Node midtime = findMiddle(sortedtime);
		insertNode(&nou, midtime->timestamp, midval->value, i + 1);
		// aici trebuie eliberata memoria
		// de pe heap de fiecare data
		// deoarece memoria de pe heap este
		// cumva alocata "intamplator"
		freeList(sortedval);
		freeList(sortedtime);
		(*nr)++;
		i++;
		start++;
		end++;
	}
	// eliberez memoria de la lista initiala si returnez lista noua
	freeList(head);
	return nou;
}

// Task 1.2.2
// filtrarea aritmetica
Node filtrareAritmetica(Node head, int n, int *nr)
{
	int k = 5;
	int start = 0;
	int end = k;
	int i = 0;
	*nr = 0;
	Node nou = NULL;
	Node p = head;
	// partea asta e explicata si mai sus
	// singura diferenta aici e ca in loc sa iau elementul median
	// nu mai sortez lista si fac media aritmetica a valorilor
	// si le introduc in lista noua
	// cum n-ul initial care simboliza numarul de elemente din lista
	// se schimba am decis sa iau un parametru nr care sa
	// simbolizeze noul numar de elemente din lista
	while(end <= n)
	{
		Node q = p;
		double averageval = 0;
		i = start;
		while(q != NULL && i < end)
		{
			averageval += q->value;
			i++;
			q = q->next;
		}
		averageval /= k;
		q = p;
		int pos = start;
		while(q != NULL && pos < end)
		{
			if(pos >= start + k / 2 && pos < end - k / 2)
			{
				insertNode(&nou, q->timestamp, averageval, (*nr) + 1);
			}
			pos++;
			q = q->next;
		}
		p = p->next;
		(*nr)++;
		start++;
		end++;
	}
	// eliberez memoria si returnez lista nou creata
	freeList(head);
	return nou;
}

// Task 1.3
// unformizarea in timp
// e destul de ad-hoc
// daca diferenta dintre 2 noduri e in intervalul [100, 1000]
// atunci schimb valoarea nodului i cu media aritmetica dintre
// valoarea de dinainte si valoarea curenta
// la sfarsit returnez lista
Node uniformizareaInTimp(Node head)
{
	Node p = head;
	p = p->next;
	while(p != NULL)
	{
		if(p->timestamp - p->prev->timestamp >= 100 && p->timestamp - p->prev->timestamp <= 1000)
		{
			p->timestamp = p->prev->timestamp + (p->timestamp - p->prev->timestamp)/2;
			p->value = (p->value + p->prev->value)/2;
		}
		p = p->next;
	}
	return head;
}

// functie de inversarea a unei liste dublu inlantuita
void reverseList(Node *head)
{
	Node p = *head;
	Node temp = NULL;
	while(p != NULL)
	{
		temp = p->prev;
		p->prev = p->next;
		p->next = temp;
		p = p->prev;
	}
	if(temp != NULL) *head = temp->prev;
}

// Task 1.4
// completarea datelor
// aici se observa ca w-urile sunt constante
// de aceea le calculez la inceput si le bag intr-o lista noua
Node completareaDatelor(Node head, int *n)
{
	int k = 3;
	int pos = 0;
	Node p = head;
	p = p->next;
	int i;
	Node w = NULL;
	double s = 0;
	for(i = 0; i < k; ++i)
		s += (double)pow((double)i/(k - 1), 2)*0.9 + 0.1;
	for(i = 0; i < k; ++i)
		insertNode(&w, 0, ((double)pow((double)i/(k - 1), 2)*0.9 + 0.1)/s, i + 1);

	// aici urmez pasii cerintei
	// daca diferenta dintre timpii a 2 noduri consecutive
	// este mai mare ca 1000 (1 secunda)
	// atunci trebuie adaugate noduri intermediare
	// imi fac lista de noduri right
	// lista de noduri left
	// si aplic formulele din cerinta
	while(p != NULL)
	{
		if(p->timestamp - p->prev->timestamp > 1000)
		{
			Node left = NULL;
			Node right = NULL;
			Node q = p;
			q = q->prev;
			int i = 0;
			int curent_timestamp = q->timestamp;
			int left_timestamp = q->timestamp;
			while(q->prev != NULL && i < k - 1)
			{
				insertNode(&left, q->timestamp, q->value, i + 1);
				i++;
				q = q->prev;
			}
			insertNode(&left, q->timestamp, q->value, i + 1);
			i = 0;
			q = p;
			int right_timestamp = q->timestamp;
			while(q != NULL && i < k)
			{
				insertNode(&right, q->timestamp, q->value, i + 1);
				i++;
				q = q->next;
			}
			
			int j;
			int sf = right->timestamp;
			reverseList(&left);
			reverseList(&right);
			double s_left = 0;
			double s_right = 0;
			Node l = left;
			Node z = w;
			while(l != NULL && z != NULL)
			{
				s_left += l->value*z->value;
				l = l->next;
				z = z->next;
			}
			Node r = right;
			z = w;
			while(r != NULL && z != NULL)
			{
				s_right += r->value*z->value;
				r = r->next;
				z = z->next;
			}
			for(j = curent_timestamp + 200; j < sf; j += 200)
			{
				double C = (double)(j - left_timestamp)/(right_timestamp - left_timestamp);
				insertNode(&head, j, (1-C)*s_left + C*s_right, pos + 1);
				pos++;
				(*n)++;
			}
			freeList(left);
			freeList(right);
		}
		pos++;
		p = p->next;
	}
	// la sfarsit returnez lista
	// desi nu era nevoie sa fie returnata dar asa ar fi trebuit
	// sa fac 2 free-uri astfel o sa fac doar unul, la final
	freeList(w);
	return head;
}

// Task 1.5 Bonus
// statistici
// si cerinta asta este ad-hoc
// trebuie sortata lista dupa care
// un numar de noduri care se afla intr-un interval dat
// de delta trebuie specificat numarul lor
// astfel o sa sortez dupa care aplic logica asta
Node Statistici(Node head, int delta)
{
	head = MergeSortVal(head);
	Node p = head;
	double x = roundf(p->value/delta)*delta;
	int bottom = (int)x;
	Node q = p;
	while(q->next != NULL) q = q->next;
	int k = 0;
	while(p != NULL)
	{
		if(p->value < bottom || p->value > (bottom + delta))
		{
			if(k != 0) printf("[%d, %d] %d\n", bottom, bottom + delta, k);
			bottom += delta;
			k = 0;
			p = p->prev;
		}
		else k++;
		p = p->next;
	}
	if(q->value >= bottom && q->value <= bottom + delta)
	{
		printf("[%d, %d] %d\n", bottom, bottom + delta, 1);
	}
	return head;
}

int main(int argc, char* argv[])
{
	int i = 0;
	if(argc == 1)
	{
		fprintf(stderr, "Nu s-au dat argumente in linia de comanda\n");
		exit(1);
	}

	Node head = NULL;
	int n;
	// citire standard
	// si introducere in lista
	scanf("%d", &n);
	int nr = 0;
	for(i = 0; i < n; ++i)
	{
		double x;
		int timestamp;
		scanf("%d%lf", &timestamp, &x);
		insertNode(&head, timestamp, x, i + 1);
	}
	int ok = 0;
	// aici aplic logica de argumente in linia de comanda
	for(i = 1; i < argc; ++i)
	{
		if(strcmp(argv[i], "--e1") == 0)
		{
			eliminareExceptii(&head, &n);
		}
		else if(strcmp(argv[i], "--e2") == 0)
		{
			nr = 0;
			head = filtrareMediana(head, n, &nr);
			n = nr;
		}
		else if(strcmp(argv[i], "--e3") == 0)
		{
			nr = 0;
			head = filtrareAritmetica(head, n, &nr);
			n = nr;
		}
		else if(strcmp(argv[i], "--u") == 0)
		{
			head = uniformizareaInTimp(head);
		}
		else if(strcmp(argv[i], "--c") == 0)
		{
			head = completareaDatelor(head, &n);
		}
		else if(argv[i][2] == 's' && argv[i][3] == 't')
		{
			ok = 1;
			int j = 0;
			char tmp[15];
			int delta = 0;
			// asa obtin numarul de dupa "--st"
			// pentru a-i atribui lui delta valoarea respectiva
			for(j = 4; j < strlen(argv[i]); ++j)
				tmp[j - 4] = argv[i][j];
			tmp[j - 4] = '\0';
			for(j = 0;j < strlen(tmp); ++j)
			{
				delta = delta*10 + tmp[j] - '0';
			}
			head = Statistici(head, delta);
		}
	}
	if(ok == 0)
	{
		printf("%d\n", n);
		print(head);
	}
	// la sfarsit eliberez memoria din lista si returnez 0
	freeList(head);
	return 0;
}