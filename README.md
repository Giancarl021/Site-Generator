# Site Generator

## Função

Gerar uma pasta com uma árvore de arquivos para um projeto de site

## Funcionamento

O arquivo exe compilado irá criar uma pasta com a data autal, no formato *YYYY-MM-DD* com a seguinte árvore de arquivos:

    [DATA ATUAL]
    ├── css
    │   └── style.css
    ├── img
    ├── js
    │   └── script.js
    └── index.html

Os arquivos **.html**, **.css** e **.js** serão gerados com um *preset* já inserido neles, sendo a origem deles os dados inseridos nos arquivos dentro da pasta *presets*

## Console

O código suporta subcomandos em um console, que modificam algumas características da pasta gerada

Para utilizar o programa no console se utiliza a sintaxe:

    NS4 [subcomandos]

Para o console funcionar, o *exe* não deve ser renomeado e o console deverá estar carregado na pasta onde o *exe* se encontra

Para facilitar, o arquivo *batch* presente no repositório inicia o console no local onde se encontra (no mesmo local do *exe*)

Os comandos suportados são:

    -add : Adiciona alguma importação ao HTML, como um framework JavaScript, por exemplo

    #[NOME DA PASTA] : Altera o nome da pasta para o nome escrito após o sustenido

    -config / -presets : Abrem a pasta com os presets do site e as referências para importação

    -php : Gera um arquivo index.php ao invés de index.html

### Comando -add

+ Sintaxe:

        -add nome_do_framework1 [nome_do_framework2 [nome_do_framework3 [...]]]

+ Função:

    Adiciona importações no local do **marcador** do preset de HTML (geralmente o marcador fica na tag **\<head>**)

+ Exemplo:

    Ao apenas clicar no exe ou executá-lo sem nenhum comando *-add*, o arquivo HTML virá assim:

        <!DOCTYPE html>
        <html lang="pt-br">

        <head>
            <meta charset="utf-8" />
            <meta http-equiv="X-UA-Compatible" content="IE=edge" />
            <title>Site</title>
            <meta name="viewport" content="width=device-width, initial-scale=1" />
            <link rel="icon" href="img/logo.png" type="image/x-icon">
            <link rel="stylesheet" type="text/css" media="screen" href="css/style.css" />
	        <!-- PRESET BY GIANCARL021 -->
            <script src="js/script.js"></script>
        </head>

        <body>

        </body>

        </html>

    Ao executar no Console o comando:

        NS4 -add jquery

    O HTML virá assim:

		<!DOCTYPE html>
		<html lang="pt-br">

		<head>
			<meta charset="utf-8" />
			<meta http-equiv="X-UA-Compatible" content="IE=edge" />
			<title>Site</title>
			<meta name="viewport" content="width=device-width, initial-scale=1" />
			<link rel="icon" href="img/logo.png" type="image/x-icon">
			<link rel="stylesheet" type="text/css" media="screen" href="css/style.css" />
			<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
			<!-- PRESET BY GIANCARL021 -->
			<script src="js/script.js"></script>
		</head>

		<body>

		</body>

		</html>

### Comando -php

+ Sintaxe:

        -php

+ Função:
    
    Troca o arquivo gerado de *index.html* para *index.php*, usando para geração do arquivo o *preset.php* no lugar de *preset.html*

### Comando \#

+ Sintaxe:

        #nome_da_pasta

+ Função:

    Renomeia a pasta para o nome digitado após o sustenido

### Comando -config / -presets

+ Sintaxe:

        -config

    ou

        -presets

+ Função:

    Abre a pasta com os arquivos de *presets* e referências de importação

## Presets

### Arquivos Base

Na pasta *presets* há os arquivos usados de base para a criação dos sites e os arquivos de referência para adicionar campos no HTML

Todos os arquivos podem ser modificados, porém **não** podem ser renomeados

Os *presets* do HTML e do PHP exclusivamente usam um **marcador** para carregar as importações ou tags com o comando *-add*, sendo este o caracter "*@*"

Este caracter só é lido uma vez, sendo assim, se o arquivo não o possuir, o código não realizará as importações. Por outro lado se este estiver presente mas não com o objetivo de importação, será necessário acrescentar outro acima para evitar erros no código

Por padrão o "*@*" está dentro da tag *\<head>*

### Pasta de referências

A pasta *_ref* possui arquivos com a extensão *.ref* que contém as linhas necessárias para importar as bibliotecas no HTML. Por padrão a pasta vêm com referências para o *jquery, angular e bootstrap*

+ É possível adicionar qualquer tipo de código em HTML para usar nas importações, basta escrevê-lo, deixar uma linha em branco no final do arquivo (para fins de formatação) e em seguida utilizá-lo no Console (usando o nome do arquivo sem extensão após o comando *-add*)

## Problemas Conhecidos

> Comando "#" não aceita espaços no nome da pasta (apenas a primeira palavra é lida)

> Ao utilizar importações com duplicatas nas importações *(jquery e bootstrap)*, o código não remove as duplicatas

> Só funciona no Windows