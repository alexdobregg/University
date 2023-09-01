int OrizVert(Punct2D *p1, Punct2D *p2) {
    // Intervalul creaza un segment vertical sau orizontal.
    int ov;
    if(p1->x == p2->x) {
        // Vertical.
        ov = 0;
    } else if(p1->y == p2->y) {
        // Orizontal.
        ov = 1;
    }
    return ov;
}

int max(int a, int b) {
    if(a < b) {
        return b;
    }
    return a;
}

int min(int a, int b) {
    if(a < b) {
        return a;
    }
    return b;
}

int intersection(Interval2D *I1, Interval2D *I2) {
    Punct2D *I11 = I1->punct_stanga;
    Punct2D *I12 = I1->punct_dreapta;
    Punct2D *I21 = I2->punct_stanga;
    Punct2D *I22 = I2->punct_dreapta;

    int ov1 = OrizVert(I11, I12);
    int ov2 = OrizVert(I21, I22);
    
    if(ov1 == 1 && ov2 == 0) {
        if((I11->y <= max(I21->y, I22->y) && I11->y >= min(I21->y, I22->y)) &&
            (I21->x <= max(I11->x, I12->x) && I21->x >= min(I11->x, I12->x))) {
            return 1;
        }
    }
    if(ov1 == 0 && ov2 == 1) {
        if((I21->y <= max(I11->y, I12->y) && I21->y >= min(I11->y, I12->y)) &&
            (I11->x <= max(I21->x, I22->x) && I11->x >= min(I21->x, I22->x))) {
            return 1;
        }
    }
    return 0;

}

int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {
    int i, j, Intersectii = 0;
    for(i = 0; i < lista->dimensiune; i++) {
        for(j = i; j < lista->dimensiune; j++) {
            Intersectii += intersection(lista->intervale[i], lista->intervale[j]);
        }
    }
    return Intersectii;
}