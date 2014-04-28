// PPCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define _pi_ 3.14

float sqroot(float n)
{
	float result;
	__asm {	
		mov eax, n
			mov ebx, eax
			mov [esp], eax
			mov [esp+4H], ebx
			mov cx, 10
petla:
		fld [esp]
		fld [esp+4H]
		fdiv
			fld [esp+4H]
		fadd
			fld1
			fld1
			fadd
			fdiv
			fstp [esp+4H]
		dec cx
			jnz petla
			mov ebx, [esp+4H]
		mov result, ebx
	}

	return(result);
}

struct urojona {
	float x1u, x2u, su, ru, ilu, iru;
};

struct rownanie {
	int *wsp;
	float d, x1r, x2r, sr, rr, ilr, irr, mz1, mz2, fi1, fi2; 
	urojona u;
};

void formatuj_rownanie(struct rownanie * e) {
	int a=e->wsp[0], b=e->wsp[1], c=e->wsp[2];
	if (a==1) printf("x^2"); else if (a==-1) printf("-x^2",a); else if(a<0) printf("%dx^2",a); else if (a>0) printf("%dx^2",a);
	if (b==1) {if(a!=0) printf("+x"); else printf("x");}else if (b==-1) printf("-x"); else if(b<0) printf("%dx",b); else if (b>0) if (a!=0) printf("+%dx",b); else printf("%dx",b);
	if(c<0) printf("%d",c); else if (c>0)  if (a!=0 || b!=0) printf("+%d",c); else printf("%d",c);
	if (a==0 && b==0 && c==0) printf("0=0"); else printf("=0");
}

float oblicz_d(struct rownanie * e) {
	e->d=(float)e->wsp[1]*e->wsp[1]-(float)4*e->wsp[0]*e->wsp[2];
	return(e->d);
}


void formatuj_zesp(float x1r, float x1u) {
	printf("\n");
	if (x1r!=0) printf("%f",x1r);
	if (x1u>0 && x1r!=0) printf("+%fi", x1u); else if(x1u>0)printf("%fi", x1u);  else if (x1u<0)printf("%fi", x1u);
}

void oblicz_pierwiastki(struct rownanie * e) {
	if (e->wsp[0]!=0) {
		float d=oblicz_d(e);
		if (d>0) {
			e->x1r=-e->wsp[1]-sqroot(d)/(2*e->wsp[0]);
			e->x2r=-e->wsp[1]+sqroot(d)/(2*e->wsp[0]);
			printf("\n2 pierwiastki x1=%f, x2=%f", e->x1r, e->x2r);
		} else if (d==0) {
			e->x1r=-e->wsp[1]/(2*e->wsp[0]);
			printf("\n1 pierwiastek xo=%f",e->x1r);

		} else {
			e->x1r=(-e->wsp[1]/(float)(2*e->wsp[0]));
			e->x2r=e->x1r;
			float absd=d<0?-d:d;
			e->u.x1u=-sqroot(absd)/(2*e->wsp[0]);
			e->u.x2u=-e->u.x1u;
			printf("\nPierwiastki zespolone:\n");
			formatuj_zesp(e->x1r, e->u.x1u);
			formatuj_zesp(e->x2r, e->u.x2u);
		}
	} else {
		if (e->wsp[1]!=0) {
			e->x1r=-e->wsp[2]/e->wsp[1]; 
			printf("\nx1=%f",e->x1r);
		} else if(e->wsp[1]==0 && e->wsp[2]!=0) printf("\nr.sprzeczne"); 
			else if (e->wsp[1]==0 && e->wsp[2]==0) printf("\nrnieoznaczone"); 
	}
}

void dodaj(struct rownanie * e) {
	if (e->d>0) e->sr=e->x1r+e->x2r; else if (e->d<0) {
		e->sr=e->x1r+e->x2r;
		e->u.su=e->u.x1u+e->u.x2u;
	}
}

void odejmij(struct rownanie * e) {
	if (e->d>0) e->rr=e->x1r-e->x2r; else if (e->d<0) {
		e->rr=e->x1r-e->x2r;
		e->u.ru=e->u.x1u-e->u.x2u;
	}	
}

void pomnoz(struct rownanie * e) {
	if (e->d>0) e->ilr=e->x1r*(e->x2r); else if (e->d<0) {
		e->ilr=e->x1r*(e->x2r)-e->u.x1u*(e->u.x2u);
		e->u.ilu=e->u.x1u*(e->u.x2u)+e->x2r*(e->u.x1u);
	}		
}

void podziel(struct rownanie * e) {
	if (e->d>0) e->irr=e->x1r/(e->x2r); else if (e->d<0) {
		e->irr=(e->x1r*(e->x2r)+e->u.x1u*(e->u.x2u))/(e->x2r*(e->x2r)+e->u.x2u*(e->u.x2u));
		e->u.iru=(e->x2r*(e->u.x1u)-e->x1r*(e->u.x2u))/(e->x2r*(e->x2r)+e->u.x2u*(e->u.x2u));
	}		
}

void wyswietl(struct rownanie * e) {
	if (e->d<0 && e->wsp[0]!=0 && e->wsp[1]!=0) {
		printf("\nSuma");
		formatuj_zesp(e->sr, e->u.su);
		printf("\nRoznica");
		formatuj_zesp(e->rr, e->u.ru);
		printf("\nIloczyn");
		formatuj_zesp(e->ilr, e->u.ilu);
		printf("\nIloraz");
		formatuj_zesp(e->irr, e->u.iru);
		printf("\n%f(cos(%f)+i*sin(%f))", e->mz1, e->fi1, e->fi1);
		printf("\n%f(cos(%f)+i*sin(%f))", e->mz2, e->fi2, e->fi2);
	} else if (e->d>0 && e->wsp[0]!=0 && e->wsp[1]!=0) {
		printf("\nSuma: %f\n", e->sr);
		printf("Roznica: %f\n", e->rr);
		printf("Iloczyn: %f\n", e->ilr);
		printf("Iloraz: %f\n", e->irr);
	}
}

void wczytaj(struct rownanie * e) {
	scanf("%d", &e->wsp[0]);
	scanf("%d", &e->wsp[1]);
	scanf("%d", &e->wsp[2]);
}

void postac_trygonometr(struct rownanie * e) {
	e->mz1=sqrt(e->x1r*e->x1r+e->u.x1u*e->u.x1u);
	e->mz2=e->mz1;
	
	if (e->x1r > 0) {
		e->fi1=atan2(e->u.x1u, e->x1r);	
	} else if (e->x1r<0 && e->u.x1u >=0) {
		e->fi1=atan2(e->u.x1u, e->x1r)+_pi_;	
	} else if (e->x1r < 0 && e->u.x1u < 0) {
		e->fi1=atan2(e->u.x1u, e->x1r)-_pi_;
	} else if (e->x1r == 0 && e->u.x1u >0) {
		e->fi1=_pi_/2;
	} else if (e->x1r == 0 && e->u.x1u < 0) {
		e->fi1=-_pi_/2;
	}


	if (e->x2r > 0) {
		e->fi2=atan2(e->u.x2u, e->x2r);	
	} else if (e->x2r<0 && e->u.x2u >=0) {
		e->fi2=atan2(e->u.x2u, e->x2r)+_pi_;	
	} else if (e->x2r < 0 && e->u.x2u < 0) {
		e->fi2=atan2(e->u.x2u, e->x2r)-_pi_;
	} else if (e->x2r == 0 && e->u.x2u >0) {
		e->fi2=_pi_/2;
	} else if (e->x2r == 0 && e->u.x2u < 0) {
		e->fi2=-_pi_/2;
	}

	e->fi1=180/_pi_*e->fi1;
	e->fi2=180/_pi_*e->fi2;

}

int _tmain(int argc, _TCHAR* argv[])
{
	int a, b, c;
	struct rownanie* e = (struct rownanie*)malloc(sizeof(struct rownanie));
	e->wsp = (int*)malloc(3*sizeof(int));
	wczytaj(e);

	formatuj_rownanie(e);
	oblicz_pierwiastki(e);

	
	dodaj(e);
	odejmij(e);
	pomnoz(e);
	podziel(e);
	

	if (e->wsp[0]!=0)
	postac_trygonometr(e);

	wyswietl(e);

	free (e->wsp);
	free (e);

	getchar(); getchar();
	return 0;
}

