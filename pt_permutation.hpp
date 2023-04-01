class pt_permutation {
    public:
        pt_permutation(int[], int);
        pt_permutation(pt_permutation&);
        int *p;
        int size;
        int min_cost;
        int p_opt;
    private:
        void calculate_min_cost_and_p_opt(int&, int&);
        void update_opt();
        void set_at(int, int);
};