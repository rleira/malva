#ifndef INC_REQ_newGA
#define INC_REQ_newGA
#include "newGA.hh"
#include <math.h>

skeleton newGA
{

	// Problem ---------------------------------------------------------------

	Problem::Problem ():_dimension(0),_maxPais(0),_precio_viaje(NULL),_tasa_temporada(NULL)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "Number of Variables " << pbm._dimension
		   << endl;

		//Imprimo el arreglo con los precios de los viajes
        os << "Matriz con precios de viajes: " << endl << endl;
        for (int i = 0; i < pbm._maxPais; i++){
            os << i << " -> ";
            for (int j = 0; j < pbm._maxPais; j++){
                os << pbm._precio_viaje[i][j] << ", ";
            }
            os << endl;
        }
        os << endl;
        return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int i, j;

        cout << "Init problem 0";

		// Inicializo tasa por temporada baja, media y alta
		pbm._tasa_temporada = new float [4];
		pbm._tasa_temporada[0] = 1.0f;
		pbm._tasa_temporada[1] = 1.1f;
		pbm._tasa_temporada[2] = 1.3f;
		pbm._tasa_temporada[3] = 1.3f;

		cout << "Init problem 1";

		pbm._dimension = 4; // Hardcoded para ej1
		pbm._maxPais = 5;

        //Pido memoria para almacenar los precios de los viajes
		pbm._precio_viaje = new int* [pbm._maxPais];
		for (i = 0; i < pbm._maxPais; i++) {
		    pbm._precio_viaje[i] = new int [pbm._maxPais];
		}

        //Cargo el archivo con los precios de los viajes
	    FILE* stream = fopen("input.txt", "r");

        char* tmp;
        char line[1024];
		for (i = 0; (i < pbm._maxPais && fgets(line, 1024, stream)); i++){
            for (j = 0; j < pbm._maxPais; j++) {
                tmp = strdup(line);
                const char * precio = pbm.getfield(tmp, j + 1);
                pbm._precio_viaje[i][j] = atoi(precio);
                free(tmp);
            }
        }

        cout << pbm;

		return is;
	}

    //Funcion para leer el con la matrix, lee una linea!
    /*
        Ejemplo:
        tmp = strdup(line);
        const char * item = pbm.getfield(tmp,1);
        int item_int=atoi(item);
        free(tmp);
    */
    const char* Problem::getfield(char* line, int num)
    {
        const char* tok;
        for (tok = strtok(line, " \t"); tok && *tok; tok = strtok(NULL, " \t\n")){
            if (!--num){
                return tok;
            }
        }
        return NULL;
    }

    int Problem::getMaxPais() const{
        return _maxPais;
    }

    int ** Problem::getPrecioViajes() const{
        return _precio_viaje;
    }

    float * Problem::getTasaTemporadas() const{
        return _tasa_temporada;
    }

	bool Problem::operator== (const Problem& pbm) const
	{
		if (_dimension!=pbm.dimension()) return false;
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
		//return maximize;
		return minimize;
	}

	int Problem::dimension() const
	{
		return _dimension;
	}

	Problem::~Problem(){
	    //Libero la memoria pedida para almacenar los precios de los viajes
        for (int i=0; i < _maxPais; i++) {
            delete[] _precio_viaje[i];
        }
        delete[] _precio_viaje;
        delete[] _tasa_temporada;
	}

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm),_var(pbm.dimension())
	{}

	const Problem& Solution::pbm() const
	{
		return _pbm;
	}

	Solution::Solution(const Solution& sol):_pbm(sol.pbm())
	{
		*this=sol;
	}

	istream& operator>> (istream& is, Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			is >> sol._var[i];
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			os << " " << sol._var[i];
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol._var.size();i++)
			ns << sol._var[i];
		return ns;
	}

	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol._var.size();i++)
			ns >> sol._var[i];
		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		_var=sol._var;
		return *this;
	}

	bool Solution::operator== (const Solution& sol) const
	{
		if (sol.pbm() != _pbm) return false;
		for(int i = 0; i < _var.size(); i++)
			if(_var[i] != sol._var[i]) return false;
		return true;
	}

	bool Solution::operator!= (const Solution& sol) const
	{
		return !(*this == sol);
	}

	void Solution::initialize()
	{
		for (int i = 0; i < _pbm.dimension(); i++) {
            _var[i] = i + 1;
		}
	}

	double Solution::fitness ()
	{
        int fitness = 0.0;
        int fromCity, toCity, viajeFitness;

        fromCity = 0;
		for (int i=0; i < pbm().dimension(); i++){
		    toCity = _var[i];

            viajeFitness = (toCity == fromCity) ? 9999 : pbm().getPrecioViajes()[fromCity][toCity];

		    // Acumulo fitness
            fitness += viajeFitness * pbm().getTasaTemporadas()[i];

            // La ciudad fromCity de la proxima iteracion es la toCity de la actual iteracion
            fromCity = toCity;
		}

		return fitness;
	}

	char *Solution::to_String() const
	{
		return (char *)_var.get_first();
	}

	void Solution::to_Solution(char *_string_)
	{
		int *ptr=(int *)_string_;
		for (int i=0;i<_pbm.dimension();i++)
		{
			_var[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return (_pbm.dimension() * sizeof(int));
	}


	int& Solution::var(const int index)
	{
		return _var[index];
	}


	Rarray<int>& Solution::array_var()
	{
		return _var;
	}

	Solution::~Solution()
	{}

	// UserStatistics -------------------------------------------------------

	UserStatistics::UserStatistics ()
	{}

	ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF TRIALS                   	 " << endl;
		os << "------------------------------------------------------------------" << endl;

		for (int i=0;i< userstat.result_trials.size();i++)
		{
			os << endl
			   << userstat.result_trials[i].trial
			   << "\t" << userstat.result_trials[i].best_cost_trial
			   << "\t\t" << userstat.result_trials[i].worst_cost_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_iteration_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].time_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_spent_trial;
		}
		os << endl << "------------------------------------------------------------------" << endl;
		return os;
	}

	UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
	{
		result_trials=userstats.result_trials;
		return (*this);
	}

	void UserStatistics::update(const Solver& solver)
	{
		if( (solver.pid()!=0) || (solver.end_trial()!=true)
		  || ((solver.current_iteration()!=solver.setup().nb_evolution_steps())
		       && !terminateQ(solver.pbm(),solver,solver.setup())))
			return;

		struct user_stat *new_stat;

		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         		 		 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.evaluations_best_found_in_trial();
		new_stat->nb_iteration_best_found_trial  = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     		 	 = solver.worst_cost_trial();
		new_stat->best_cost_trial     		 	 = solver.best_cost_trial();
		new_stat->time_best_found_trial		 	 = solver.time_best_found_trial();
		new_stat->time_spent_trial 		 		 = solver.time_spent_trial();

		result_trials.append(*new_stat);
	}

	void UserStatistics::clear()
	{
		result_trials.remove();
	}

	UserStatistics::~UserStatistics()
	{
		result_trials.remove();
	}

// Intra_operator  --------------------------------------------------------------

	Intra_Operator::Intra_Operator(const unsigned int _number_op):_number_operator(_number_op),probability(NULL)
	{}

	unsigned int Intra_Operator::number_operator() const
	{
		return _number_operator;
	}

	Intra_Operator *Intra_Operator::create(const unsigned int _number_op)
	{
		switch (_number_op)
		{
			case 0: return new Crossover;break;
			case 1: return new Mutation();break;
		}
	}

	ostream& operator<< (ostream& os, const Intra_Operator& intra)
	{
		switch (intra.number_operator())
		{
			case 0: os << (Crossover&)intra;break;
			case 1: os << (Mutation&)intra;break;
		}
		return os;
	}

	Intra_Operator::~Intra_Operator()
	{}

//  Crossover:Intra_operator -------------------------------------------------------------

	Crossover::Crossover():Intra_Operator(0)
	{
		probability = new float[1];
	}

	void Crossover::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
	    //Usamos cruzamiento de dos puntos (2PX)
        int i=0;
        Rarray<int> aux(sol1.pbm().dimension());
        aux=sol2.array_var();

        int limit=rand_int((sol1.pbm().dimension()/2)+1,sol1.pbm().dimension()-1);
        int limit2=rand_int(0,limit-1);

        for (i=0;i<limit2;i++)
            sol2.var(i)=sol1.var(i);
        for (i=0;i<limit2;i++)
            sol1.var(i)=aux[i];
        for (i=limit;i<sol1.pbm().dimension();i++)
            sol2.var(i)=sol1.var(i);
        for (i=limit;i<sol1.pbm().dimension();i++)
            sol1.var(i)=aux[i];
	}

	void Crossover::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover&  cross)
	{
		 os << "Crossover." << " Probability: "
                    << cross.probability[0]
		    << endl;
		 return os;
	}

	void Crossover::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_crossover_probability",(char *)probability,1,sizeof(float));
	}

	void Crossover::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_crossover_probability",(char *)probability,nbytes,length);
	}

	void Crossover::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover::~Crossover()
	{
		delete [] probability;
	}

	//  Mutation: Sub_operator -------------------------------------------------------------

	Mutation::Mutation():Intra_Operator(1)
	{
		probability = new float[2];
	}

	void Mutation::mutate(Solution& sol) const {

	    // Aqui usamos EXCHANGE MUTATION

	    // Para almacenar valor anterior a mutacion y poder aplicar 'Exchange mutation'
	    int oldValue;

		for (int i = 0; i < sol.pbm().dimension(); i++) {
            if (rand01() <= probability[1]) {
                // Guardo valor anterior
                oldValue = sol.var(i);
                //La mutacion intercambia un gen aleatoriamente con probabilidad uniforme
                sol.var(i) = rand_int(1,4);
                if (oldValue != sol.var(i)) {
                    for (int j = 0; j < sol.pbm().dimension(); j++) {
                        if ((j != i) && (sol.var(j) == sol.var(i))) {
                            sol.var(j) = oldValue;
                            break;
                        }
                    }
                }
            }
        }
	}

	void Mutation::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i<sols.size();i++)
			if(rand01() <= probability[0])	mutate(*sols[i]);
	}

	ostream& operator<< (ostream& os, const Mutation&  mutation)
	{
		os << "Mutation." << " Probability: " << mutation.probability[0]
		   << " Probability1: " << mutation.probability[1]
		   << endl;
		return os;
	}

	void Mutation::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f %f ",&op,&probability[0],&probability[1]);
		assert(probability[0]>=0);
		assert(probability[1]>=0);
	}

	void Mutation::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_mutation_probability",(char *)probability,2,sizeof(probability));
	}

	void Mutation::UpdateFromState(const StateCenter& _sc)
	{
		unsigned long nbytes,length;
		_sc.get_contents_state_variable("_mutation_probability",(char *)probability,nbytes,length);
	}

	Mutation::~Mutation()
	{
		delete [] probability;
	}

// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition(),_mejor_precio_intinerario(2147483646)
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
	    int nuevo_mejor_precio_intinerario = solver.best_cost_trial();
	    if (nuevo_mejor_precio_intinerario < _mejor_precio_intinerario) {
	        _mejor_precio_intinerario = nuevo_mejor_precio_intinerario;
            FILE * pFile;
			pFile = fopen ("output_intinerario.csv", "w");
			for (int i = 0; i < pbm.dimension(); i++){
				fprintf (pFile, "Desde %i -> hasta: %i\n",i+1,solver.best_solution_trial().var(i));
			}
			fprintf (pFile, "Suma total intinerario, %d\n", _mejor_precio_intinerario);
			fclose (pFile);
	    }
		return false;
		//return ((int)solver.best_cost_trial() == pbm.dimension());
	}

	StopCondition_1::~StopCondition_1()
	{}

	//------------------------------------------------------------------------
	// Specific methods ------------------------------------------------------
	//------------------------------------------------------------------------

	bool terminateQ (const Problem& pbm, const Solver& solver,
			 const SetUpParams& setup)
	{
		StopCondition_1 stop;
		return stop.EvaluateCondition(pbm,solver,setup);
	}
}
#endif

