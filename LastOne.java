/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package jipp;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import org.apache.commons.math3.complex.Complex;

/**
 *
 * @author i2070p
 */
abstract class Dzialanie<T> {

    public Dzialanie() {
    }

    public abstract T pomnoz(T t1, T t2);

    public abstract T podziel(T t1, T t2);

    public abstract T dodaj(T t1, T t2);

    public abstract T odejmij(T t1, T t2);
}

class DzialanieNorm extends Dzialanie<Float> {

    public DzialanieNorm() {

    }

    @Override
    public Float pomnoz(Float t1, Float t2) {
        return (t1 * t2);
    }

    @Override
    public Float podziel(Float t1, Float t2) {
        return (t1 / t2);
    }

    @Override
    public Float dodaj(Float t1, Float t2) {
        return (t1 + t2);
    }

    @Override
    public Float odejmij(Float t1, Float t2) {
        return (t1 - t2);
    }

}

class DzialanieZesp extends Dzialanie<Complex> {

    public DzialanieZesp() {

    }

    @Override
    public Complex pomnoz(Complex t1, Complex t2) {
        return (t1.multiply(t2));
    }

    @Override
    public Complex podziel(Complex t1, Complex t2) {
        return (t1.divide(t2));
    }

    @Override
    public Complex dodaj(Complex t1, Complex t2) {
        return (t1.add(t2));
    }

    @Override
    public Complex odejmij(Complex t1, Complex t2) {
        return (t1.subtract(t2));
    }

}

class Rownanie {

    public Rownanie(int a, int b, int c) {
        this.a = a;
        this.b = b;
        this.c = c;
    }

    public String formatuj_rownanie() {
        StringBuilder ss = new StringBuilder();
        if (a == 1) {
            ss.append("x^2");
        } else if (a == -1) {
            ss.append("-x^2");
        } else if (a < 0) {
            ss.append(a).append("x^2");
        } else if (a > 0) {
            ss.append(a).append("x^2");
        }
        if (b == 1) {
            if (a != 0) {
                ss.append("+x");
            } else {
                ss.append("x");
            }
        } else if (b == -1) {
            ss.append("-x");
        } else if (b < 0) {
            ss.append(b).append("x");
        } else if (b > 0) {
            if (a != 0) {
                ss.append("+").append(b).append("x");
            } else {
                ss.append(b).append("x");
            }
        }
        if (c < 0) {
            ss.append(c);
        } else if (c > 0) {
            if (a != 0 || b != 0) {
                ss.append("+").append(c);
            } else {
                ss.append(c);
            }
        }
        if (a == 0 && b == 0 && c == 0) {
            ss.append("0=0");
        } else {
            ss.append("=0");
        }
        return (ss.toString());
    }

    private void oblicz_d() {
        d = (float) b * b - (float) 4 * a * c;
        if (d >= 0) {
            dzialanie = new DzialanieNorm();

        } else if (d < 0) {
            dzialanie = new DzialanieZesp();
        }

    }

    public String oblicz_pierwiastki() {

        StringBuilder ss = new StringBuilder();
        if (a != 0) {
            oblicz_d();
            if (d > 0) {

                p1 = (float) -b - Math.sqrt(d) / (2 * a);
                p2 = (float) -b + Math.sqrt(d) / (2 * a);
                ss.append("\n2 pierwiastki x1=").append(p1).append(", x2=").append(p2);
            } else if (d == 0) {
                p1 = (float) -b / (2 * a);
                p2 = (float) 0;
                ss.append("\n1 pierwiastek xo=").append(p1);
            } else {
                float absd = d < 0 ? -d : d;
                p1 = new Complex(-b / (float) (2 * a), -Math.sqrt(absd) / (2 * a));
                p2 = new Complex(-b / (float) (2 * a), -Math.sqrt(absd) / (2 * a));
                ss.append("\nPierwiastki zespolone:\n").append(p1.toString()).append("\n").append(p2.toString());
            }
        } else {
            dzialanie = new DzialanieNorm();
            p1 = (float) 0;
            p2 = (float) 0;
            if (b != 0) {
                p1 = (float) -c / b;

                ss.append("\nx1=").append(p1);
            } else if (b == 0 && c != 0) {
                ss.append("\nr.sprzeczne");
            } else if (b == 0 && c == 0) {
                ss.append("\nrnieoznaczone");
            }
        }

        return (ss.toString());
    }

    public String postac_trygonometr() {
        StringBuilder ss = new StringBuilder();
        if (d != null && d < 0) {
            Complex u1 = (Complex) p1;
            Complex u2 = (Complex) p2;
            double u1arg = 180.0 / Math.PI * u1.getArgument();
            double u2arg = 180.0 / Math.PI * u2.getArgument();
            double z1mz2 = u1.abs() * u2.abs();
            double z1dz2 = u1.abs() / u2.abs();
            double fi1 = u1.getArgument();
            double fi2 = u2.getArgument();
            ss.append("\n").append(u1.abs()).append("(cos(").append(u1arg).append(")+i*sin(").append(u1arg).append("))");
            ss.append("\n").append(u2.abs()).append("(cos(").append(u2arg).append(")+i*sin(").append(u2arg).append("))");
            ss.append("\nz1*z2=").append(z1mz2).append("(cos(").append(fi1 + fi2).append(")+i*sin(").append(fi1 + fi2).append("))");
            ss.append("\nz1/z2=").append(z1dz2).append("(cos(").append(fi1 - fi2).append(")+i*sin(").append(fi1 - fi2).append("))");
        } else {
            ss.append("Liczba nie jest zespolona.");
        }

        return (ss.toString());
    }

    public String pobierzWyniki() {
        StringBuilder sb = new StringBuilder();
        if (d != null) {
            if (d < 0 && a != 0 && b != 0) {
                sb.append("\nSuma").append(sum.toString())
                        .append("\nRoznica")
                        .append(roz.toString())
                        .append(postac_trygonometr());
            } else if (d > 0 && a != 0 && b != 0) {
                sb.append("\nSuma:").append(sum + "\n")
                        .append("Roznica:" + roz + "\n");
            }
            if (d > 0) {
                sb.append("\nIloczyn")
                        .append(ilocz.toString())
                        .append("\nIloraz")
                        .append(ilor.toString());
            }
        } else {
            sb.append("");
        }
        return (sb.toString());
    }

    public void odejmij() {
        roz = dzialanie.odejmij(p1, p2);
    }

    public void dodaj() {

        sum = dzialanie.dodaj(p1, p2);
    }

    public void podziel() {
        ilor = dzialanie.podziel(p1, p2);
    }

    public void pomnoz() {
        ilocz = dzialanie.pomnoz(p1, p2);
    }

    private int a, b, c;
    private Dzialanie dzialanie;
    private Float d;
    private Object ilocz, ilor;
    private Object p1, p2, sum, roz;
}

public class JIPP {

    public static void main(String[] args) {
        Scanner reader = new Scanner(System.in);
        List<String> histList = new ArrayList<>();

        while (true) {
            System.out.println("wprowadz a, b, c: ");
            int a = reader.nextInt();
            int b = reader.nextInt();
            int c = reader.nextInt();

            Rownanie e = new Rownanie(a, b, c);
            StringBuilder log = new StringBuilder();

            log.append(e.formatuj_rownanie()).append(e.oblicz_pierwiastki());
            e.dodaj();
            e.odejmij();
            e.podziel();
            e.pomnoz();
            log.append(e.pobierzWyniki());
            histList.add(log.toString());
            System.out.println(log.toString());
            System.out.println("1 - wyjscie, 2 - historia, cokolwiek innego - nastepne rownanie");
            int opt = reader.nextInt();
            switch(opt) {
                case 1: System.exit(0);
                    break;
                case 2: 
                    System.out.println("****HISTORIA*****");
                    for (String s : histList) {
                        System.out.println(s+"\n\n");
                    }
                    break;
            }
            
        }
    }

}
