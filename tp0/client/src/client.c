#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	logger = log_create("tp0.log","TP0",true, LOG_LEVEL_INFO);
	log_info(logger,"Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	
	config = config_create("cliente.config");
	
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config,"IP");
	puerto= config_get_string_value(config,"PUERTO");


	// Loggeamos el valor de config
	log_info(logger,valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	//BEGIN - HANDSHAKE

	uint32_t handshake = 1;
	uint32_t result;

	send(conexion, &handshake, sizeof(uint32_t),NULL );
	recv(conexion, &result, sizeof(uint32_t), MSG_WAITALL);

	if(result == 0){
		log_info(logger,"---HANDSHAKE CON SERVIDOR OK---");
	}else{
		log_info(logger,"---ERROR EN HANDSHAKE---");
	}

	//END	- HANDSHAKE

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion,logger);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(*leido){
		log_info(logger,leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion, t_log* logger)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	int tamanio_leido = 0;
	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(*leido){
		paquete = crear_paquete();
		log_info(logger,leido);
		tamanio_leido = sizeof(char) * string_length(leido);
		agregar_a_paquete(paquete,leido,tamanio_leido);
		enviar_paquete(paquete,conexion);
		eliminar_paquete(paquete);
		leido = readline("> ");
	}
	//enviar_paquete(paquete,conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	//eliminar_paquete(paquete);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	close(conexion);
	//config_destroy(config);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
