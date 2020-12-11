#include "cache.h"
#include "test_suite.h"
#include <stdio.h>

bool prueba0_init(){
	tamanio_cache = 4;
	tamanio_bloque = 256;
	cantidad_vias = 4;
	init();
	destroy();
	return true;
}

/* Pruebas find_set() */

bool prueba1_MismoSetDistintoTag(){
	tamanio_cache = 4;
	tamanio_bloque = 32;
	cantidad_vias = 2;
	init();
	bool paso = (find_set(0xABCD) == find_set(0xBBCD));
	destroy();
	return paso;
}

bool prueba2_DistintoIndexDistintoSet(){
	tamanio_cache = 4;
	tamanio_bloque = 32;
	cantidad_vias = 2;
	init();
	bool paso = (find_set(0xABCD) != find_set(0xB4CD));
	destroy();
	return paso;
}

bool prueba3_Direccion0x0000VaAlSetCero(){
	tamanio_cache = 4;
	tamanio_bloque = 32;
	cantidad_vias = 2;
	init();
	bool paso = (find_set(0x0000) == 0);
	destroy();
	return paso;
}

bool prueba4_Direccion0xFFFFVaAlSet63(){
	tamanio_cache = 4;
	tamanio_bloque = 32;
	cantidad_vias = 2;
	init();
	bool paso = (find_set(0xFFFF) == 63);
	destroy();
	return paso;
}

bool prueba5_Direccion0xF12DVaAlSet18(){
	tamanio_cache = 1;
	tamanio_bloque = 16;
	cantidad_vias = 1;
	init();
	bool paso = (find_set(0xF12D) == 18);
	destroy();
	return paso;
}

bool prueba6_Direccion0xF342VaAlSet3(){
	tamanio_cache = 8;
	tamanio_bloque = 256;
	cantidad_vias = 8;
	init();
	bool paso = (find_set(0xF342) == 3);
	destroy();
	return paso;
}

/* Pruebas find_lru() */

bool prueba7_seEncuentraCorrectamenteElLRUEntre4BloquesValidos(){
	tamanio_cache = 1;
	tamanio_bloque = 256;
	cantidad_vias = 4;
	init();
	cache.vias[0].bloques[0].valido = true;
	cache.vias[0].bloques[0].distancia_lru = 3;

	cache.vias[1].bloques[0].valido = true;
	cache.vias[1].bloques[0].distancia_lru = 4;

	cache.vias[2].bloques[0].valido = true;
	cache.vias[2].bloques[0].distancia_lru = 2;

	cache.vias[3].bloques[0].valido = true;
	cache.vias[3].bloques[0].distancia_lru = 1;

	bool paso = (find_lru(0) == 1);
	destroy();
	return paso;
}

bool prueba8_siUnBloqueNoEsValidoEntoncesEsElBloqueQueDevuelveFindLRU(){
	tamanio_cache = 1;
	tamanio_bloque = 256;
	cantidad_vias = 4;
	init();
	cache.vias[0].bloques[0].valido = true;
	cache.vias[0].bloques[0].distancia_lru = 3;

	cache.vias[1].bloques[0].valido = true;
	cache.vias[1].bloques[0].distancia_lru = 4;

	cache.vias[2].bloques[0].valido = false;

	cache.vias[3].bloques[0].valido = true;
	cache.vias[3].bloques[0].distancia_lru = 1;

	bool paso = (find_lru(0) == 2);
	destroy();
	return paso;
}

bool prueba10_LRUEnCacheConSoloUnaViaDevuelveCero(){
	tamanio_cache = 1;
	tamanio_bloque = 4;
	cantidad_vias = 1;
	init();
	cache.vias[0].bloques[0].valido = true;
	cache.vias[0].bloques[0].distancia_lru = 3;

	bool paso = (find_lru(0) == 0);
	destroy();
	return paso;
}

/* Pruebas read_block() */

bool prueba9_leerUnBloqueDeMemoriaPrincipalCargaCorrectamenteElBloqueEnCache(){
	tamanio_cache = 1;
	tamanio_bloque = 4;
	cantidad_vias = 1;
	/*
	 * #b_offset = 2;
	 * #b_index = 8;
	 * #b_tag = 6;
	 */
	init();
	memoria_ppal[40] = 'H';
	memoria_ppal[41] = 'O';
	memoria_ppal[42] = 'L';
	memoria_ppal[43] = 'A';
	/* 40 = 0x28 = 101000 */

	read_block(40 >> 2);
	bool paso = true;
	paso = paso && (cache.vias[0].bloques[0xA].datos[0] == 'H');
	paso = paso && (cache.vias[0].bloques[0xA].datos[1] == 'O');
	paso = paso && (cache.vias[0].bloques[0xA].datos[2] == 'L');
	paso = paso && (cache.vias[0].bloques[0xA].datos[3] == 'A');
	paso = paso && (cache.vias[0].bloques[0xA].direccion == 0x28);
	destroy();
	return paso;
}


/* Pruebas write_block() */

bool prueba11_CuandoSeEscribeUnBloqueEnMemoriaPpalEstaContieneLosDatosDeCache(){
	tamanio_cache = 1;
	tamanio_bloque = 4;
	cantidad_vias = 1;
	/*
	 * #b_offset = 2;
	 * #b_index = 8;
	 * #b_tag = 6;
	 */
	init();
	cache.vias[0].bloques[0xA].direccion = 0x28;
	cache.vias[0].bloques[0xA].dirty = true;
	cache.vias[0].bloques[0xA].valido = true;
	cache.vias[0].bloques[0xA].datos[0] = 'H';
	cache.vias[0].bloques[0xA].datos[1] = 'O';
	cache.vias[0].bloques[0xA].datos[2] = 'L';
	cache.vias[0].bloques[0xA].datos[3] = 'A';
	write_block(0, 0xA);

	bool paso = true;

	paso = paso && (memoria_ppal[40] == 'H');
	paso = paso && (memoria_ppal[41] == 'O');
	paso = paso && (memoria_ppal[42] == 'L');
	paso = paso && (memoria_ppal[43] == 'A');

	destroy();
	return paso;
}



void tests_init_destroy(){
	test_suite_nuevo_grupo("Pruebas de init() y destroy()");
	test_suite_afirmar(prueba0_init(),"Prueba de memory leaks.");
}

void tests_findSet(){
	test_suite_nuevo_grupo("Pruebas de find_set()");
	/* Pruebas de caja negra */
	test_suite_informar("Configuracion: cs=4, bs=32, w=2");
	test_suite_afirmar(prueba1_MismoSetDistintoTag(),"Distintas direcciones de memoria con mismo index tienen el mismo set.");
	test_suite_afirmar(prueba2_DistintoIndexDistintoSet(),"Distintas direcciones de memoria con distinto index tienen distinto set.");
	test_suite_afirmar(prueba3_Direccion0x0000VaAlSetCero(),"La direccion 0x0000 mapea al set 0");
	test_suite_afirmar(prueba4_Direccion0xFFFFVaAlSet63(),"La direccion 0xFFFF mapea al set 63");
	test_suite_informar("Configuracion: cs=1, bs=16, w=1");
	test_suite_afirmar(prueba5_Direccion0xF12DVaAlSet18(),"La direccion 0xF12D mapea al set 18");
	test_suite_informar("Configuracion: cs=8, bs=256, w=8");
	test_suite_afirmar(prueba6_Direccion0xF342VaAlSet3(),"La direccion 0xF342 mapea al set 3");
}

void tests_findLRU(){
	test_suite_nuevo_grupo("Pruebas de find_LRU()");
	/* Pruebas de caja blanca */
	test_suite_informar("Configuracion: cs=1, bs:256B, w = 4");
	test_suite_afirmar(prueba7_seEncuentraCorrectamenteElLRUEntre4BloquesValidos(), "Se encuentra correctamente el bloque LRU");
	test_suite_afirmar(prueba8_siUnBloqueNoEsValidoEntoncesEsElBloqueQueDevuelveFindLRU(), "Si un bloque es invalido, este es el bloque LRU");
	/* Pruebas de caja negra */
	test_suite_afirmar(prueba10_LRUEnCacheConSoloUnaViaDevuelveCero(), "Si sólo hay una vía, el lru es 0.");
}

void tests_readBlock(){
	test_suite_nuevo_grupo("Pruebas de read_block()");
	/* Pruebas de caja blanca */
	test_suite_informar("Configuracion: cs=1, bs:4B, w = 1");
	test_suite_afirmar(prueba9_leerUnBloqueDeMemoriaPrincipalCargaCorrectamenteElBloqueEnCache(), "Se lee un bloque de memoria ram, este coincide con lo esperado.");
}

void tests_write_block(){
	test_suite_nuevo_grupo("Pruebas de write_block()");
	/* Pruebas de caja blanca */
	test_suite_informar("Configuracion: cs=1, bs:4B, w = 1");
	test_suite_afirmar(prueba11_CuandoSeEscribeUnBloqueEnMemoriaPpalEstaContieneLosDatosDeCache(), "Se escribe un bloque de cache en memoria ppal y esta es sobreescrita.");

}

int main(int argc, char const *argv[]){
	tests_init_destroy();
	tests_findSet();
	tests_findLRU();
	tests_readBlock();
	tests_write_block();
	test_suite_mostrar_reporte();
	return 0;
}