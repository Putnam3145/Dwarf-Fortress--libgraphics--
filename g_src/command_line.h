struct command_linest {
	string original;
	stringvectst arg_vect;

	long gen_id = -1;
	unsigned long world_seed;
	char use_seed = 0;
	string world_param;
	char use_param = 0;

	void init(const string &str);
	char grab_arg(string &source,long &pos);
	void handle_arg(string &arg);
};