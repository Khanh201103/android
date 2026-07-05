package navis.calculator;

/** Simple calculator interface exposed through Binder. */
interface ICalculator
{
    void init();
    double add(double x, double y);
    double sub(double x, double y);
    double mul(double x, double y);

    /** Returns x / y. Throws IllegalArgumentException when y is zero. */
    double div(double x, double y);
}
