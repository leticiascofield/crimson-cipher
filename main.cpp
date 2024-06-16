#include <iostream>
#include <vector>
#include <bitset>
#include <tuple>
#include <algorithm>

using namespace std;

const int MASK_DIREITA = 1 << 0;
const int MASK_CIMA    = 1 << 1;
const int MASK_ESQUERDA = 1 << 2;
const int MASK_BAIXO   = 1 << 3;

class Cristal {
public:
    int x, y, brilho, conexoes;
};

bool comparaCristais(const Cristal& c1, const Cristal& c2) {
    if (c1.x == c2.x) {
        return c1.y < c2.y;
    }
    return c1.x < c2.x;
}

int buscaCristal(const vector<Cristal>& cristais, int x, int y) {
    int esq = 0;
    int dir = cristais.size() - 1;
    while (esq <= dir) {
        int meio = (esq + dir) / 2;
        if (cristais[meio].x == x && cristais[meio].y == y) {
            return meio;
        } else if (cristais[meio].x < x || (cristais[meio].x == x && cristais[meio].y < y)) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }
    return -1;
}

int CalculoPedrasEscolhidas(int linha, int maskAnterior, int primeiraMask, int linhas, int colunas, vector<vector<vector<int>>>& dp, vector<vector<int>>& valor, vector<vector<vector<int>>>& escolha, vector<int>& bitmaskPedrasExistentes, const vector<Cristal>& cristais) {
    if (linha == linhas) {
        for (int i = 0; i < cristais.size(); ++i) {
            const Cristal& cristal = cristais[i];
            if ((maskAnterior & (1 << cristal.y)) && (primeiraMask & (1 << cristal.y)) && (cristal.conexoes & MASK_CIMA)) {
                return -1e9;
            }
        }
        return 0;
    }
    if (dp[linha][maskAnterior][primeiraMask] != -1) return dp[linha][maskAnterior][primeiraMask];

    int res = -1e9;
    for (int maskAtual = 0; maskAtual < (1 << colunas); ++maskAtual) {
        bool conflito = false;
        for (int j = 0; j < colunas; ++j) {
            if (maskAtual & (1 << j)) {
                int idx = buscaCristal(cristais, linha, j);
                if (idx != -1) {
                    const Cristal& cristal = cristais[idx];
                    if ((maskAnterior & (1 << cristal.y)) && (cristal.conexoes & MASK_CIMA)) {
                        conflito = true;
                        break;
                    }
                    if ((maskAtual & (1 << ((cristal.y - 1 + colunas) % colunas))) && (cristal.conexoes & MASK_ESQUERDA)) {
                        conflito = true;
                        break;
                    }
                    if ((maskAtual & (1 << ((cristal.y + 1) % colunas))) && (cristal.conexoes & MASK_DIREITA)) {
                        conflito = true;
                        break;
                    }
                }} } if (conflito || (maskAtual & ~bitmaskPedrasExistentes[linha])) continue; if (linha && (maskAtual & primeiraMask)) { for (int j = 0; j < colunas; ++j) { if ((maskAtual & (1 << j)) && (primeiraMask & (1 << j))) { int idx = buscaCristal(cristais, linha, j); if (idx != -1 && (cristais[idx].conexoes & MASK_BAIXO)) { conflito = true; break; } } } if (conflito) continue; }

    int novoRes = CalculoPedrasEscolhidas(linha + 1, maskAtual, primeiraMask, linhas, colunas, dp, valor, escolha, bitmaskPedrasExistentes, cristais) + valor[linha][maskAtual];
    if (novoRes > res) {
        res = novoRes;
        escolha[linha][maskAnterior][primeiraMask] = maskAtual;
    }
}

return dp[linha][maskAnterior][primeiraMask] = res;
}

void preencheValor(vector<vector<int>>& valor, const vector<Cristal>& cristais, int n, int colunas) { for (int i = 0; i < n; ++i) { for (int mask = 0; mask < (1 << colunas); ++mask) { if (mask & (1 << cristais[i].y)) { valor[cristais[i].x][mask] += cristais[i].brilho; } } } }

int main() { 
    // Entrada 
    int linhas, colunas, n; scanf("%d %d %d", &linhas, &colunas, &n); vector<Cristal> cristais(n); vector<int> bitmaskPedrasExistentes(linhas, 0);

for (int i = 0; i < n; ++i) {
    int direita, cima, esquerda, baixo;
    scanf("%d %d %d %d %d %d %d", &cristais[i].x, &cristais[i].y, &cristais[i].brilho,
          &direita, &cima, &esquerda, &baixo);
    cristais[i].x--;
    cristais[i].y--;

    int conexoes = 0;
    if (direita) conexoes |= MASK_DIREITA;
    if (cima) conexoes |= MASK_CIMA;
    if (esquerda) conexoes |= MASK_ESQUERDA;
    if (baixo) conexoes |= MASK_BAIXO;

    cristais[i].conexoes = conexoes;

    bitmaskPedrasExistentes[cristais[i].x] |= (1 << cristais[i].y);
}

sort(cristais.begin(), cristais.end(), comparaCristais);

vector<vector<vector<int>>> dp(linhas, vector<vector<int>>(1 << colunas, vector<int>(1 << colunas, -1)));
vector<vector<vector<int>>> escolha = dp;

vector<vector<int>> valor(linhas, vector<int>(1 << colunas, 0));
preencheValor(valor, cristais, n, colunas);

// Saída
int res = 0;
int melhorMaskInicial = 0;
for (int mask = 0; mask < (1 << colunas); ++mask) {
    int novoRes = CalculoPedrasEscolhidas(0, mask, mask, linhas, colunas, dp, valor, escolha, bitmaskPedrasExistentes, cristais);
    if(novoRes > res) { res = novoRes; melhorMaskInicial = mask; } }

vector<pair<int, int>> pedrasEscolhidas;
int maskAnterior = melhorMaskInicial;
for (int i = 0; i < linhas; ++i) {
    int maskAtual = escolha[i][maskAnterior][melhorMaskInicial];
    for (int j = 0; j < colunas; ++j) {
        if (maskAtual & (1 << j)) {
            pedrasEscolhidas.push_back({i + 1, j + 1});
        }
    }
    maskAnterior = maskAtual;
}

// Saída
printf("%ld %d\n", pedrasEscolhidas.size(), res);
for (const auto& p : pedrasEscolhidas) {
    printf("%d %d\n", p.first, p.second);
}

return 0;
}