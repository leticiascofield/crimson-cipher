#include <iostream>
#include <vector>
#include <bitset>
#include <tuple>
#include <algorithm>

using namespace std;

class Cristal {
public:
    int x, y, brilho, direita, cima, esquerda, baixo;
};

int CalculoPedrasEscolhidas(int linha, int maskAnterior, int primeiraMask, int linhas, int colunas, vector<vector<vector<int>>>& dp, vector<vector<int>>& valor, vector<vector<vector<int>>>& escolha, vector<int>& bitmaskPedrasExistentes) {
    if (linha == linhas) {
        if ((maskAnterior & primeiraMask) || (maskAnterior & (maskAnterior >> 1)) || (maskAnterior & (maskAnterior << 1)) || (maskAnterior & (maskAnterior << (colunas - 1))) || ((maskAnterior & 1) && (maskAnterior & (1 << (colunas - 1))))) return -1e9;
        return 0;
    }
    if (dp[linha][maskAnterior][primeiraMask] != -1) return dp[linha][maskAnterior][primeiraMask];

    int res = -1e9;
    for (int maskAtual = 0; maskAtual < (1 << colunas); ++maskAtual) {
        if ((maskAtual & maskAnterior) || (maskAtual & (maskAtual >> 1)) || (maskAtual & (maskAtual << 1)) || (maskAtual & (maskAtual << (colunas - 1))) || ((maskAtual & 1) && (maskAtual & (1 << (colunas - 1)))) || (maskAtual & ~bitmaskPedrasExistentes[linha])) continue;
        if (linha && (maskAtual & primeiraMask)) continue;

        // Verificar se os cristais nas extremidades da linha estão ambos ativados
        if ((maskAtual & 1) && (maskAtual & (1 << (colunas - 1)))) continue;

        int novoRes = CalculoPedrasEscolhidas(linha + 1, maskAtual, primeiraMask, linhas, colunas, dp, valor, escolha, bitmaskPedrasExistentes) + valor[linha][maskAtual];
        if (novoRes > res) {
            res = novoRes;
            escolha[linha][maskAnterior][primeiraMask] = maskAtual;
        }
    }

    return dp[linha][maskAnterior][primeiraMask] = res;
}

void preencheValor(vector<vector<int>>& valor, const vector<Cristal>& cristais, int n, int colunas) {
    for (int i = 0; i < n; ++i) { //preenche os valores de todas as combinações por linha
        for (int mask = 0; mask < (1 << colunas); ++mask) {
            if (mask & (1 << cristais[i].y)) {
                valor[cristais[i].x][mask] += cristais[i].brilho;
            }
        }
    }
}

int main (){

    //Entrada
    int linhas, colunas, n;
    scanf("%d %d %d", &linhas, &colunas, &n);

    vector<Cristal> cristais(n);
    vector<int> bitmaskPedrasExistentes(linhas, 0);

    for(int i = 0; i < n; ++i){
        scanf("%d %d %d %d %d %d %d", &cristais[i].x, &cristais[i].y, &cristais[i].brilho,
             &cristais[i].direita, &cristais[i].cima, &cristais[i].esquerda, &cristais[i].baixo);
        cristais[i].x--;
        cristais[i].y--;

        // Definir o bit na posição y do vetor bitmaskPedrasExistentes[x]
        bitmaskPedrasExistentes[cristais[i].x] |= (1 << cristais[i].y);
    }

    vector<int> bitmaskPedrasEscolhidas(linhas, 0);

    vector<vector<vector<int>>> dp = vector<vector<vector<int>>>(linhas, vector<vector<int>>(1 << colunas, vector<int>(1 << colunas, -1)));
    vector<vector<vector<int>>> escolha = dp;

    vector<vector<int>> valor = vector<vector<int>>(linhas, vector<int>(1 << colunas));
    preencheValor(valor, cristais, n, colunas);

    vector<int> bitmaskConexoes(linhas, 0);
    for(int i = 0; i < n; ++i){
        int mask = 0;
        if (cristais[i].direita) mask |= (1 << ((cristais[i].y + 1) % colunas));
        if (cristais[i].esquerda) mask |= (1 << ((cristais[i].y - 1 + colunas) % colunas));
        bitmaskConexoes[cristais[i].x] |= mask;
    }

    // Chame aqui: Função para conseguir a melhor configuração possivel, de acordo com as restrições e querendo o valor máximo de brilho

    //Saída

    int res = 0;
    int melhorMaskInicial = 0;
    for (int mask = 0; mask < (1 << colunas); ++mask) {
        int novoRes = CalculoPedrasEscolhidas(0, mask, mask, linhas, colunas, dp, valor, escolha, bitmaskPedrasExistentes);
        if (novoRes > res) {
            res = novoRes;
            melhorMaskInicial = mask;
        }
    }

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

    //Saída
    cout << pedrasEscolhidas.size() << " " << res << endl;
    for (const auto& p : pedrasEscolhidas) {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}