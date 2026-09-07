# Grupo 19

# Geração de Solos de Guitarra com Grafos

Projeto da disciplina de Grafos que usa algoritmos de caminhos mínimos e árvore
geradora mínima para criar um solo de guitarra a partir de uma progressão de
acordes. O programa escolhe posições de arpejos que possam ser tocadas com
transições pequenas no braço da guitarra e exporta o resultado em arquivos MIDI.

## Alunos

| Matrícula | Nome |
| --- | --- |
| 241025990 | Pedro Henrique Ferreira Xavier |
| 241040350 | Philipe Amancio Reis Caetano |

## Vídeo de apresentação

**[Vídeo de apresentação](https://youtu.be/WtbMEarn7IU)**

## Objetivos

- Representar uma progressão de acordes como um grafo em camadas.
- Gerar diferentes posições de cada arpejo no braço da guitarra.
- Encontrar, com Dijkstra, uma sequência de arpejos com menor custo de transição.
- Criar variações do solo-base e selecionar algumas delas com Prim.
- Exportar o solo original e as variações em arquivos MIDI.

## Funcionamento geral

O fluxo do programa é o seguinte:

1. A progressão de acordes é recebida pela linha de comando ou usa uma
   progressão padrão.
2. Cada acorde é convertido em notas MIDI e são geradas várias posições
	possíveis para tocá-lo na guitarra.
3. Essas posições são organizadas em camadas: cada camada corresponde a um
	acorde da progressão.
4. As arestas ligam posições de acordes consecutivos. O peso representa o
	esforço para mudar de uma posição para outra.
5. O algoritmo de Dijkstra encontra o caminho de menor custo entre o primeiro
	e o último acorde. Esse caminho é o solo-base.
6. A partir do solo-base são criadas quatro variações, como inversão das notas,
	deslocamento de região e adição de nota de aproximação.
7. Um grafo de variações é construído e o algoritmo de Prim calcula uma árvore
	geradora mínima entre as variações.
8. Algumas variações são selecionadas e todos os resultados são exportados em
	arquivos MIDI.

## Modelagem do grafo do solo

O grafo principal é definido em `ArpeggioGraph`:

- **Vértices:** representam posições possíveis de um arpejo para um acorde.
- **Camadas:** cada acorde da progressão possui sua própria camada de vértices.
- **Arestas:** ligam arpejos de uma camada ao acorde seguinte.
- **Peso:** combina distância entre casas, distância entre cordas e uma
  penalidade para saltos maiores que quatro casas.
- **Source:** vértice inicial, ligado aos arpejos do primeiro acorde.
- **Sink:** vértice final, ligado aos arpejos do último acorde.

O peso de uma transição é calculado em `transitionCost`. Quanto menor o peso,
mais confortável tende a ser a mudança entre os dois arpejos.

## Algoritmos

### Dijkstra

Implementado em `src/dijkstra_solo.cpp`, o Dijkstra percorre o grafo usando uma
fila de prioridade. Ele mantém:

- `dist`: menor custo conhecido para chegar a cada vértice;
- `parent`: vértice anterior no melhor caminho encontrado;
- `priority_queue`: escolhe o próximo vértice com menor custo.

Ao chegar ao `sink`, o algoritmo reconstrói o caminho usando `parent` e retorna
os arpejos escolhidos. Como os pesos são não negativos, o caminho encontrado é
um caminho mínimo.

### Prim

Implementado em `src/prim_variations.cpp`, o Prim é aplicado ao grafo das
variações. A distância entre duas variações considera:

- diferença entre as regiões do braço;
- diferença entre notas MIDI correspondentes;
- diferença entre casas;
- diferença na quantidade de notas.

O resultado é uma árvore geradora mínima. A função `sampleVariations` usa as
arestas dessa árvore para selecionar até três variações para exportação.

### Complexidade dos algoritmos

Considere `V` como o número de vértices do grafo e `E` como o número de
arestas. Como os algoritmos usam `priority_queue`, suas complexidades são:

| Etapa ou algoritmo | Complexidade de tempo | Complexidade de memória |
| --- | --- | --- |
| Dijkstra com fila de prioridade | `O((V + E) log V)` | `O(V + E)` |
| Prim com fila de prioridade | `O((V + E) log V)` | `O(V + E)` |
| Reconstrução do caminho do Dijkstra | `O(V)` | `O(V)` |
| Construção do grafo de variações | `O(K^2 * L)` | `O(K^2 + K * L)` |

Na construção do grafo de variações, `K` é o número de solos candidatos e `L` é
o tamanho médio de um solo. O termo `K^2` aparece porque cada par de variações
é comparado. No projeto atual, `K` é pequeno, pois são criadas apenas cinco
opções: o solo-base e quatro variações.

Na construção do grafo principal, cada posição de arpejo de uma camada é
comparada com as posições da camada seguinte. Se `A_i` for a quantidade de
arpejos gerada para o acorde `i`, essa etapa cria até
`A_i * A_(i+1)` arestas entre duas camadas consecutivas. Portanto, o custo
depende principalmente da quantidade de posições possíveis para cada acorde.
Também existe um custo de geração dos arpejos, pois o código testa combinações
de posições das notas no braço da guitarra.

## Organização dos arquivos

```text
.
├── CMakeLists.txt             configuração de compilação com CMake
├── Makefile                   compilação alternativa com Make
├── README.md                  documentação do projeto
├── include/
│   ├── arpeggio.h             geração e representação de arpejos
│   ├── chords.h               leitura de acordes e seus intervalos
│   ├── dijkstra_solo.h        interface do Dijkstra
│   ├── fretboard.h            notas, cordas e casas da guitarra
│   ├── graph.h                grafo da progressão de acordes
│   ├── midi_export.h          interface da exportação MIDI
│   ├── midi_writer.h          escrita de eventos MIDI
│   └── prim_variations.h      grafo de variações e algoritmo de Prim
└── src/
    ├── arpeggio.cpp           geração das posições possíveis dos arpejos
    ├── chords.cpp             interpretação dos nomes dos acordes
    ├── dijkstra_solo.cpp      caminho mínimo do solo-base
    ├── fretboard.cpp          conversão de notas e posições na guitarra
    ├── graph.cpp              construção do grafo e custo das transições
    ├── main.cpp               fluxo principal da aplicação
    ├── midi_export.cpp        conversão do solo para arquivo MIDI
    ├── midi_writer.cpp        escrita do formato MIDI
    └── prim_variations.cpp    variações, Prim e amostragem dos resultados
```

## Requisitos

- Compilador C++ com suporte a C++17, como `g++` ou MSVC.
- CMake 3.10 ou superior, caso seja usada a configuração CMake.
- `make`, caso seja usado o Makefile.

Não são necessárias bibliotecas externas de grafos. As estruturas de grafos e
os algoritmos Dijkstra e Prim estão implementados no próprio projeto.

## Compilação com CMake

Na raiz do projeto, execute:

```bash
cmake -S . -B build
cmake --build build
```

O executável gerado pelo CMake se chama `guitar_solo`. No Windows, ele costuma
ficar em `build/Debug/guitar_solo.exe` quando o gerador padrão é Visual Studio.

## Compilação com Make

Com um ambiente que possua `make` e `g++`:

```bash
make
```

Esse comando gera `main.exe`. Para executar pelo Makefile:

```bash
make run
```

Para remover os arquivos intermediários:

```bash
make clean
```

## Execução

Sem argumentos, o programa usa a progressão padrão:

```bash
./main.exe
```

Também é possível informar os acordes diretamente:

```bash
./main.exe Am7 D7 Gmaj7 Cmaj7
```

Exemplo equivalente usando o executável do CMake no Windows:

```powershell
.\build\Debug\guitar_solo.exe Am7 D7 Gmaj7 Cmaj7
```

Os nomes dos acordes devem seguir o formato aceito pelo parser do projeto,
como `C`, `Am`, `D7`, `Gmaj7` e `Cmaj7`.

## Arquivos gerados

Durante a execução, o programa imprime no terminal:

- a progressão recebida;
- os arpejos escolhidos pelo Dijkstra;
- as notas, cordas e casas do solo-base;
- as variações selecionadas pelo Prim.

Também são gerados arquivos MIDI:

```text
build/dijkstra_ori.mid
build/prim_variation1.mid
build/prim_variation2.mid
build/prim_variation3.mid
```

O arquivo `dijkstra_ori.mid` contém o solo-base. Os demais contêm as variações
selecionadas a partir da árvore geradora mínima.

## Exemplo de saída conceitual

Para a entrada:

```text
Am7 -> D7 -> Gmaj7 -> Cmaj7
```

o programa pode escolher uma posição de `Am7` na região da casa 5, uma posição
de `D7` próxima, depois uma posição de `Gmaj7` e finalmente uma posição de
`Cmaj7`. A escolha exata depende das posições válidas geradas e dos pesos das
transições.

## Limitações e observações

- O algoritmo escolhe uma sequência de arpejos, não uma melodia nota a nota.
- O custo modela proximidade no braço, mas não considera ritmo, velocidade ou
  técnica individual do guitarrista.
- As variações são transformações simples sobre o solo-base e servem como
  material inicial para exploração musical.
- Para que os arquivos MIDI sejam criados corretamente, o diretório `build`
  deve existir antes da execução quando ele não tiver sido criado pelo CMake.

## Referências aos algoritmos

- Dijkstra: caminho mínimo em grafos com pesos não negativos.
- Prim: árvore geradora mínima em grafo ponderado não direcionado.
- Lista de adjacência: representação usada para armazenar as arestas do grafo.






