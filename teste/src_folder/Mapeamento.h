//TODO fazer fim_cidade
//TODO tirar verificacao do 3 checkpoint de forma que fique facil voltar
//TODO tirar entrada do deadend
//TODO reverificar calibragem
/*TODO verificar todos os lugares onde ele chama o metodo realinhar,
 * em algum lugar o robo viu cores diferentes com os sensores e entrou em um loop onde
 * ele ficava somente realinhando
 */

#ifndef MAPEAMENTO_H_
#define MAPEAMENTO_H_

#include "Controlador_robo.h"
#include "Sensor_cor_hsv.h"
#include "Arquivos_mapeamento.h"
#include  "Ultrassom_nxt.h"

#define virar_direita(i) robo->girar(-90)

#define virar_esquerda(i) robo->girar(90)


#define distancia_boneco 5 //MODIFICAR DEPOIS
#define delay_f 5 //Usado na detecção do final da cidade


using namespace std;


class Mapeamento {
public:
	Mapeamento(Controlador_robo *, Sensor_cor_hsv *, Ultrassom_nxt *, Ultrassom_nxt *);

	/*
	 *  Método de controle da jornada do robo pela "cidade"
	 *  segura a execucao ate terminar a cidade
	 */
	void mapear();

	/* Métodos de controle das Threads de mapeamento dos bonecos (sensor ultrassom)*/
	bool inicializar_threads_ultra();
	bool finalizar_threads_ultra();




private:
	Controlador_robo *robo;
	Sensor_cor_hsv *sensor;
	Ultrassom_nxt *ultraE;
	Ultrassom_nxt *ultraD;

	// quando o robo estiver saindo de um lado ele volta e realinha
	void realinha(direcao lado_saindo);
	// quando o robo chegar em uma intersec (vermelho, verde, azul ou preto)
	void intersec();

	/* Método de mapemento das direções das intersecções*/
	void mapeamento_intersec();


	/* Método utilizado em: mapeamento_intersec
	 * Se o robô passar por uma interssecção onde a mesma já foi previamente mapeada
	 * ele segue pelo "caminho certo"
	 */
	void caminho_certo();


	/* Método utilizado em: mapeamento_intersec
	 * Determina se foi encontrado o final da cidade
	 * representado por uma rampa com uma sequência de cores
	 */
	bool fim_da_cidade();


	/*verifica se a cor que o robo esta sobre quando chamar o metodo ja foi mapeada*/
	bool cor_ja_mapeada();

	/* Variável de controle de start do mapeamento dos bonecos */
	bool map_boneco_inicio = false;

	/* Arquivo para salvar as informações de mapeamento*/
	Arquivos_mapeamento *arq_map;


	/*************************flags controle do mapeamento******************************/
	/***********************************************************************************/
	// cor que o referente a intersecao que o robo acabou de sair
	Cor cor_atual = Cor::ndCor;

	// direcao referente a intersecao que o robo acabou de sair
	direcao direcao_atual = direcao::ndDirecao;

	// direcao correta de cada intersecao
	direcao_checkpoint cp = {direcao::ndDirecao, direcao::ndDirecao, direcao::ndDirecao};

	//	intersecao atual ja foi mapeada? se nao,
	//  quando encontrar uma nova intersecao mapear a atual
	bool confirmacao_status = false;

	// robo esta voltando de um dead-end?
	bool dead_end = false;

	//flags para salvar ultima leitura da cor quando necessario
	Cor cor_E = Cor::ndCor;
	Cor	cor_D = Cor::ndCor;

	// flag que controla o comportamento do robo de acordo com onde ele esta
	estados_Mapeamento estd = estados_Mapeamento::faixa;


	/********************Mapeamento dos bonecos************************/
	/* == 1 se estiver indo do ponto de start para a rampa
	 * == -1 se estiver indo da rampa para o ponto de start
	 */
	int sentido_navegacao = 0;


	/* Variável de controle de posicao na chegada de um quadrado(deadend/intersec)*/
	double posicao_inicialt = 0;

	/* Variável de controle de correção de rota*/
	int delay = 0;

	/* Variável de controle da thread*/
	bool thread_rodando_bonecos = false;

	/* Uma thread para cada sensor ultrassom: esquerdo e direito*/
	thread mapeamento_bonecoE;
	thread mapeamento_bonecoD;

	/* Métodos das threads*/
	void loop_mapeamento_bonecoE();
	void loop_mapeamento_bonecoD();

	/* Lista de Vectors e seus iteradores
	 * Os Vectors armazenam as posições dos bonecos em relação à intersecção
	 * Cada nó trata-se de uma intersecção
	 * Cada intersecção deve conter as posições dos bonecos antes e depois dela (se houver)
	 */
	list<no_intersec> no;
	list<no_intersec>::iterator it_no_atual = no.begin();
	list<no_intersec>::iterator it_no_anterior = no.begin();

	/* Controlador de posição do vector em loop_mapeamento_boneco*/
	unsigned j = 0;

	/* Demarca se o robo está dentro de uma interseccao ou não*/
	bool interseccao = false;

	/* Controla se houve leitura de bonecos entre uma intersecção e outra*/
	bool leu_boneco = false;

	/* Distância total entre uma intersecção e outra*/
	double posicao_intersec = 0.0;



};


#endif
