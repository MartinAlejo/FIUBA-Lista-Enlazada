#include "lista.h"
#include "testing.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void pruebas_generales(void) {
	lista_t *lista = lista_crear();
	bool todo_ok = true;
	int *dato_1 = malloc(sizeof(int));
	int *dato_2 = malloc(sizeof(int));
	int *dato_3 = malloc(sizeof(int));
	int *dato_4 = malloc(sizeof(int));
	if (!lista_esta_vacia(lista)) todo_ok = false;
	lista_insertar_primero(lista, dato_1);
	if (lista_esta_vacia(lista)) todo_ok = false;
	if (lista_ver_primero(lista) != dato_1) todo_ok = false;
	lista_insertar_primero(lista, dato_2);
	if (lista_ver_primero(lista) != dato_2) todo_ok = false;
	if (lista_ver_ultimo(lista) != dato_1) todo_ok = false;
	lista_insertar_ultimo(lista, dato_3);
	lista_insertar_ultimo(lista, dato_4);
	if (lista_borrar_primero(lista) != dato_2) todo_ok = false;
	if (lista_ver_primero(lista) != dato_1) todo_ok = false;
	if (lista_ver_ultimo(lista) != dato_4) todo_ok = false;
	lista_destruir(lista, free);
	free(dato_2); // Vemos que a dato_2 no se le aplico free en lista destruir, ya que no pertenecia a la lista
	print_test("Supero las pruebas generales", todo_ok);
}

void _destruir_arreglo_punteros_enteros(void **arreglo) {
    for (int i = 0; i < 2; i++) {
        free(arreglo[i]);
    }
    free(arreglo);
}

void destruir_arreglo_punteros_enteros(void *arreglo) {
    _destruir_arreglo_punteros_enteros((void **)arreglo);
}

void prueba_destruir_lista_y_arreglo(void) {
    lista_t *lista = lista_crear();
    void **arreglo = calloc(2, sizeof(void*));
    int *elem_1 = malloc(sizeof(int));
    *elem_1 = 3;
    int *elem_2 = malloc(sizeof(int));
    *elem_2 = 5;
    arreglo[0] = elem_1;
    arreglo[1] = elem_2;
    lista_insertar_ultimo(lista, arreglo);
    lista_destruir(lista, destruir_arreglo_punteros_enteros);
    // Si realizo un 'printf' puedo verificar que lo que contienen ambas variables ahora es basura
    // printf("Elemento 1: %d, Elemento 2: %d\n", *elem_1, *elem_2);
    print_test("Se destruyo una lista y el arreglo que se habia insertado", 1);
    // Se elimino la memoria utilizada en la prueba
}

void prueba_insertar_principio_iter_externo(void) {
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	int dato = 5;
	lista_iter_insertar(iter, &dato);
	print_test("Insertar un elemento en un iterador nuevo, lo inserta al principio", *(int*)lista_ver_primero(lista) == 5 && *(int*)lista_ver_ultimo(lista) == 5);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_insertar_final_iter_externo(void) {
	lista_t *lista = lista_crear();
	int dato_1 = 1;
	int dato_2 = 3;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_iter_t *iter = lista_iter_crear(lista);
	int dato_final = 5;
	while (!lista_iter_al_final(iter)) {
		lista_iter_avanzar(iter);
	}
	lista_iter_insertar(iter, &dato_final);
	print_test("Insertar un elemento cuando el iterador esta al final, lo inserta al final", *(int*)lista_ver_primero(lista) == 1 && *(int*)lista_ver_ultimo(lista) == 5);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_insertar_medio_iter_externo(void) {
	lista_t *lista = lista_crear();
	int dato_1 = 1;
	int dato_2 = 3;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_iter_t *iter = lista_iter_crear(lista);
	int dato_medio = 5;
	lista_iter_avanzar(iter);
	lista_iter_insertar(iter, &dato_medio);
	print_test("Insertar un elemento cuando el iterador esta en el medio, lo inserta en el medio", *(int*)lista_ver_primero(lista) == 1 && *(int*)lista_ver_ultimo(lista) == 3 && *(int*)lista_iter_ver_actual(iter) == 5);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_remover_iter_cambia_primero(void) {
	lista_t *lista = lista_crear();
	int dato_1 = 1;
	int dato_2 = 3;
	int dato_3 = 5;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_insertar_ultimo(lista, &dato_3);
	lista_iter_t *iter = lista_iter_crear(lista);
	int borrado = *(int*)lista_iter_borrar(iter);
	print_test("Remover un elemento en un iterador recien creado, cambia el primero", *(int*)lista_ver_primero(lista) == 3 && borrado == 1);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_remover_iter_cambia_ultimo(void) {
	lista_t *lista = lista_crear();
	int dato_1 = 1;
	int dato_2 = 3;
	int dato_3 = 5;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_insertar_ultimo(lista, &dato_3);
	lista_iter_t *iter = lista_iter_crear(lista);
	while (lista_iter_ver_actual(iter) != lista_ver_ultimo(lista)) {
		lista_iter_avanzar(iter);
	}
	int borrado = *(int*)lista_iter_borrar(iter);
	print_test("Remover un elemento con el iterador al final, cambia el ultimo", *(int*)lista_ver_ultimo(lista) == 3 && borrado == 5);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_remover_iter_medio(void) {
	lista_t *lista = lista_crear();
	int dato_1 = 1;
	int dato_2 = 3;
	int dato_3 = 5;
	bool se_borro = true;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_insertar_ultimo(lista, &dato_3);
	lista_iter_t *iter = lista_iter_crear(lista);
	lista_iter_avanzar(iter);
	int borrado = *(int*)lista_iter_borrar(iter);
	lista_iter_destruir(iter);
	lista_iter_t *nuevo_iter = lista_iter_crear(lista);
	while (!lista_iter_al_final(nuevo_iter)) {
		if (*(int*)lista_iter_ver_actual(nuevo_iter) == borrado) {
			se_borro = false;
		}
		lista_iter_avanzar(nuevo_iter);
	}
	print_test("Remover un elemento con el iterador al final, cambia el ultimo", *(int*)lista_ver_ultimo(lista) == 5 && borrado == 3 && se_borro);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(nuevo_iter);
	lista_destruir(lista, NULL);
}

void prueba_borrar_iter_lista_vacia_devuelve_null(void) {
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	void *se_pudo_borrar = lista_iter_borrar(iter);
	print_test("Remover un elemento con el iterador en una lista vacia devuelve NULL", !se_pudo_borrar);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_avanzar_iter_lista_vacia_devuelve_false(void) {
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	bool se_pudo_avanzar = lista_iter_avanzar(iter);
	print_test("Remover un elemento con el iterador en una lista vacia devuelve false", !se_pudo_avanzar);
	// Libero la memoria utilizada en la prueba
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}


bool sumar_1_primeros_3(void* dato, void* extra) {
	if (*(int*)extra >= 3) {
		return false;
	}
	*(int*)dato += 1;
	*(int*)extra += 1;
	return true;
}


bool sumar_1_todos(void* dato, void* extra) {
	*(int*)dato += 1;
	*(int*)extra += *(int*)dato;
	return true;
}

void prueba_iterador_interno_sin_corte(void) {
	lista_t *lista = lista_crear();
	void *suma_total = malloc(sizeof(int));
	*(int*)suma_total = 0;
	int dato_1 = 1;
	int dato_2 = 3;
	int dato_3 = 5;
	int dato_4 = 6;
	int dato_5 = 9;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_insertar_ultimo(lista, &dato_3);
	lista_insertar_ultimo(lista, &dato_4);
	lista_insertar_ultimo(lista, &dato_5);
	lista_iterar(lista, sumar_1_todos, suma_total);
	print_test("Iterador interno sin corte recorrio todos los elementos y les sumo 1", *(int*)suma_total == 29);
	// Libero la memoria utilizada en la prueba
	free(suma_total);
	lista_destruir(lista, NULL);
}

void prueba_iterador_interno_con_corte(void) {
	lista_t *lista = lista_crear();
	void *recorridos = malloc(sizeof(int));
	*(int*)recorridos = 0;
	int dato_1 = 1;
	int dato_2 = 3;
	int dato_3 = 5;
	int dato_4 = 6;
	int dato_5 = 9;
	lista_insertar_ultimo(lista, &dato_1);
	lista_insertar_ultimo(lista, &dato_2);
	lista_insertar_ultimo(lista, &dato_3);
	lista_insertar_ultimo(lista, &dato_4);
	lista_insertar_ultimo(lista, &dato_5);
	lista_iterar(lista, sumar_1_primeros_3, recorridos);
	print_test("Iterador interno con corte recorrio los primeros 3 elementos y les sumo 1", *(int*)recorridos == 3);
	// Libero la memoria utilizada en la prueba
	free(recorridos);
	lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante(void) {
	pruebas_generales();
	prueba_destruir_lista_y_arreglo();
	prueba_insertar_principio_iter_externo();
	prueba_insertar_final_iter_externo();
	prueba_insertar_medio_iter_externo();
	prueba_remover_iter_cambia_primero();
	prueba_remover_iter_cambia_ultimo();
	prueba_remover_iter_medio();
	prueba_borrar_iter_lista_vacia_devuelve_null();
	prueba_avanzar_iter_lista_vacia_devuelve_false();
	prueba_iterador_interno_sin_corte();
	prueba_iterador_interno_con_corte();
}

#ifndef CORRECTOR

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;
}

#endif