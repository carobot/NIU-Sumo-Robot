class dummy {
    private:
        int var1;
        char var2;
    public:
        int get_var1 () {
            return (dummy::var1);
        }

        char get_var2 () {
            return (dummy::var2);
        }
};