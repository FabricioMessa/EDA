# Estructuras de Datos Avanzados

## Lectura sugerida 1

### Estructuras de datos multidimensionales: Repaso y Expectativas

#### 1. Introducción

##### Contexto y Evolución

- El diseño y análisis de estructuras de datos y algoritmos es fundamental en la informática. Se mencionan avances significativos que van desde la búsqueda binaria hasta algoritmos polinomiales para la programación lineal.
- Estos avances han permitido generar soluciones óptimas para problemas del mundo real convirtiendo este campo en un eje central de la investigación actual.

##### De lo Unidimensional a lo Multidimensional

- La búsqueda en colecciones ordenadas de **una sola dimensión** está bien resuelta con estructuras tradicionales como árboles binarios, tablas hash y árboles multi-camino.
- Sin embargo, en áreas como robótica, estadística, geometría computacional y recuperación de información, el problema central es determinar puntos dentro de límites específicos en **múltiples dimensiones** (consultas de rango).

##### Complejidad de las Estructuras Multidimensionales

- Las estructuras de datos multidimensionales no son extensiones “naturales” o simples de las unidimensionales, son más complejas y están orientadas a problemas específicos.
- Para comparar, menciona que un árbol binario equilibrado (unidimensional) tiene un costo de construcción de O(N log N) y un costo de almacenamiento de O(N), permitiendo búsquedas de rango en un tiempo de O(logN + $K_1$). El texto aclara que no existen estructuras multidimensionales que logren un rendimiento similar de forma tan sencilla.

##### Enfoque del artículo

- Se mencionan diversas estructuras estudiadas recientemente para búsquedas de rango, como los **árboles k-d**, quadtrees, árboles de rango y árboles de atributos múltiples (MAT).
- El artículo se centra específicamente en el árbol de búsqueda k-dimensional, que es una generalización para elementos que son tuplas de k dimensiones.
- Introduce el concepto de **elementos con pasos** (números reales positivos asociados a cada dato) y adelanta que explorará la relación entre estos árboles con peso y los árboles multidireccionales equilibrados.

##### 1.1 Organización y alcance del documento

El enfoque inicial es presentar y analizar estructuras de árboles multidimensionales balanceados que sean simples, eficientes y útiles para aplicaciones en tiempo real.

##### Temas clave del contenido

- Generalización del Balanceo: Se presenta un paradigma para llevar el concepto de “árbol balanceado” (típico de una dimensión) a múltiples dimensiones.
- Manejo de violaciones: Estrategias para soportar y corregir “violaciones de estructura” cuando los datos no encajan perfectamente en el modelo generalizado.
- Árboles de atributos múltiples (MAT): La última parte del documento se dedica exclusivamente a profundizar en este tipo de estructuras.

#### 2. Qué es una estructura de datos multidimensionales

##### Definición Básica

- Dato unidimensional: Se caracteriza por un solo atributo o valor clave (ej. un número de DNI). Ejemplos: árboles AVL, B-trees, tablas hash.
- Dato multidimensional: Se caracteriza por k atributos. Se puede imaginar como un “punto” en un espacio de k dimensiones. Ejemplo: un punto geográfico tiene 2 atributos (latitud, longitud).

##### Recuperación Asociativa

Es el problema de recuperar todos los registros que satisfacen una consulta que involucra múltiples atributos a la vez. El texto aclara que ninguna estructura de índice es óptima para todas las circunstancias, ya que el rendimiento depende de:

- Características lógicas de los datos.
- Complejidad de la consulta.
- Parámetros del dispositivo físico de almacenamiento.

##### Matrices de Rendimiento

Para evaluar estas estructuras, se utilizan tres medidas principales:

- P(N) Costo de Preprocesamiento: El tiempo que toma construir la estructura con N puntos.
- S(N): Costo de Almacenamiento: El espacio (memoria/disco) necesario para guardar la estructura.
- Q(N): Costo de Acceso (Consulta): El tiempo para responder a una búsqueda o realizar modificaciones (inserciones/eliminaciones).

##### Estructuras Estáticas vs Dinámicas

- Estática: Se construyen una sola vez. Permiten consultas rápidas pero no permiten insertar ni borrar datos después.
- Dinámicas: Permiten insertar y borrar datos sobre la marcha.
  - *El problema:* Las inserciones y eliminaciones suelen destruir el “balanceo” de la estructura, lo cual es muy difícil de mantener de forma óptima comparado con el caso estático. 

##### 2.1 Una clasificación de problemas de búsqueda multidimensional

El texto define el problema de búsqueda como una función $P:T_1 \times 2^{T_2} \rightarrow T_3$, donde:

- $T_1$ es la consulta (lo que buscas).
- $T_2$ es el conjunto de datos (donde buscas).
- $T_3$ es la respuesta (si existe, el rango de puntos, etc).

##### Tipos de problemas y estructuras clave

El artículo compara cómo diferentes estructuras resuelven la búsqueda de rango (encontrar puntos dentro de un área):

| Estructuras     |                        Concepto Clave                        |                          Fortalezas                          |
| :-------------- | :----------------------------------------------------------: | :----------------------------------------------------------: |
| Point Quadtrees |  Divide el espacio en $2^{k}$ cuadrantes de forma recursiva  | Eficiente en la práctica, aunque su peor caso no es el mejor. |
| K-d tree        | Árbol binario que divide el espacio usando un atributo diferente en cada nivel. |   Muy usado; costo de consulta $O(N^{1-\frac{1}{k}}+K_1)$.   |
| Rango tree      | Árbol de búsqueda donde cada nodo tiene asociado otro árbol de menor dimensión. | El más rápido en búsquedas ($log^{k}N+K_1$), pero muy costoso en memoria y preprocesamiento. |
|                 |                                                              |                                                              |

##### Comparación vs Digital (La gran diferencia)

El texto hace una distinción vital para tu carrera en Ciencias de la Computación:

- Basadas en comparación (k-d trees, range trees): divide el espacio en regiones más pequeñas de la misma dimensiónalidad.
- Basadas en búsqueda digital (MAT, k-B trees): reducen la dimensionalidad una por una en cada nivel de árbol.

##### Áreas de aplicación y “Dinamización”

- Geometría computacional: se usa para problemas de proximidad, cierres convexos e intersecciones.
- Recuperación de información: menciona el uso de *grid files*, listas invertidas y mapas de bits comprimidos para manejar datos en memoria secundaria (discos).
- Dinamización: Es el proceso de convertir una estructura estática en una dinámica (que soporte insert/delete). El gran reto es que las inserciones suelen destruir el agrupamiento de datos en las “páginas” del disco, afectando el rendimiento.

Resumen: Si buscas velocidad extrema de consulta, usa un Range Tree, si buscas un equilibrio entre memoria y velocidad, un k-d tree o Quadtree son mejores opciones.

#### 3. Árboles equilibrados multidimensionales y ponderados

##### Antecedentes

El autor menciona que se está trabajando en una nueva dirección para crear árboles de búsqueda que sean balanceados y auto-organizables. Lo más importante aquí es:

- Simplicidad: Se buscan algoritmos que sean fáciles de entender e implementar/
- El paradigma EQSON: Se introduce un nuevo concepto para manejar “violaciones” en la estructura del árbol mediante algo llamado subárbol-EQSON. Esto permite que el árbol se mantenga eficiente incluso cuando los datos multidimensionales so complejos.

##### Árboles de búsqueda k-dimensionales

Aquí se describe una generalización del árbol de búsqueda tradicional, pero aplicando a datos de k-dimensiones. Fíjate en la figura, es un árbol ternario (cada nodo puede tener hasta 3 hijos):

- Hijo izquierdo: Contiene elementos donde el k-ésimo atributo es menor que el de la raíz.
- Hijo derecho: Contiene elementos donde el k-ésimo atributo es mayor que el de la raíz.
- Subárbol central (EQSON): Este es el punto clave. Aquí se guardan los elementos que tienen el mismo valor en el k-ésimo atributo. Para estos elementos, se construye un árbol de (K-1) dimensión (se “ignora” el atributo que ya es igual y se organiza por los demás).

##### Rendimiento y Balanceo

- El problema: Si no se controla, el rendimiento de un árbol k-dimensional en el peor de los casos puede ser muy malo.
- La solución: Un árbol k-dimensional balanceado es aquel cuya altura está restringida a $O(logn+k)$.
- Ventaja: Está altura es óptima y permite que las actualizaciones (insertar o eliminar) se realicen en un tiempo logarítmico, lo cual es muy rápido para aplicaciones en tiempo real.

Resumen: La gran diferencia con los árboles binarios normales es ese **hijo central**. En lugar de solo ir a izquierda o derecha, si los valores son iguales, el árbol “baja” una dimensión para seguir organizando el dato por sus otros atributos.

![](/private/var/folders/38/k7fql2x55qq127jbry_wpszr0000gn/T/TemporaryItems/com.apple.Photos.NSItemProvider/uuid=3343AEE2-9B14-4B23-873C-BF62EDB04CFD&code=001&library=1&type=1&mode=1&loc=true&cap=true.png/Imagen 24-03-26 a las 7.23 a. m..png)

##### Árboles pesados

Este apartado introduce el concepto de árboles con peso, que es una evolución de los árboles multidimensionales que ya vimos, pero añadiendo una capa de “importancia” a los datos.

##### ¿Qué es el “peso” en un dato?

Un elemento de datos multidimensionales pueden tener asociado un número real positivo llamado peso. La interpretación más común es que este peso representa la probabilidad de acceso:

- Más peso: el dato se consulta con mucha frecuencia.
- Menos peso: el dato casi no se consulta.

##### Árboles auto-organizables (Self-organizing trees)

Cuando un árbol multidimensional adapta su estructura dinámicamente según estos pesos (patrones de acceso), se llama árbol auto-organizable.

-> Esto es muy útil en bases de datos: los datos que más pides “suben” o se vuelven más fáciles de alcanzar, optimizando el tiempo de respuesta.

##### Nuevas operaciones 

Para gestionar estos pesos, el artículo introduce dos operaciones adicionales a las típicas de insertar/borrar:

- Promote (Promover): aumenta el peso de un elemento (porque se volvió más popular o importante)
- Demote (Degradar): Disminuye el peso de un elemento.

##### Complejidad y Rendimiento

El mejor tiempo de acceso que se puede esperar en el peor de los casos para un árbol auto-organizable es logarítmico, definido por la fórmula:
$$
O(log(\frac{W}{w_i})+k)
$$
Donde:

- $w_i$: es el peso del elemento específico que buscas.
- $W$: es la suma total de los pesos de todos los elementos en el árbol.
- $k$: son las dimensiones.

Conclusión: Un árbol de este tipo, si está bien balanceado, garantizo que tanto las búsquedas como las actualizaciones se mantengan en tiempos logarítmicos, lo cual es ideal para sistemas que cambien constantemente.

##### Árboles multidimensionales y árboles pesados

Este apartado es breve pero fundamental, ya que aplica la relación y conversión entre los árboles con peso y los árboles multidimensionales. Este artículo plantea que estas dos familias de árboles no están aisladas, de hecho, puedes usar una para construir la otra de dos formas principales:

- De 1D con peso a Multidireccional:
  - Puedes tomar una estructura de árbol unidimensional que ya sea eficiente con pesos y usarla como base para implementar los nodos de un TRIE (una estructura de recuperación de información).
  - Esto permite “escalar” la eficiencia de una sola dimensión hacia estructuras de múltiples dimensiones.
- De ($k+1$) dimensiones a k dimensiones con peso:
  - Esta es la parte más ingeniosa: puedes usar un árbol de ($k+1$) dimensiones para crear un árbol de k dimensiones que maneje pesos.
  - ¿Cómo? Utilizando los subárboles del nivel más bajo como “subárboles virtuales”. Estos subárboles no representan datos extra, sino que representan los pesos de los elementos de las k dimensiones superiores.

##### El concepto de Tiempo amortizado

- En muchas aplicaciones, lo que importa no es cuánto tarda una única operación aislada sino cuánto tarda una secuencia de operaciones.
- El tiempo amortizado es el promedio de tiempo que tarda una operación a lo largo de una secuencia de peor caso. Es decir, aunque una operación específica sea costosa, si la mayoría son muy baratas, el “costo promedio” (amortizado) es bajo.

##### ¿Cuándo importa el peor caso?

Aunque el tiempo amortizado es una buena medida general, el artículo aclara que el peor caso individual sigue siendo crítico en ciertos escenarios:

- Aplicaciones de tiempo real: donde cada operación debe responder dentro de un límite estricto de tiempo.
- Como base de otras estructuras: a veces, tener un buen rendimiento de peor caso en una estructura simple es necesario para asegurar que una estructura más compleja basada en ella funcione bien de forma amortizada.

##### Ejemplos de Estructuras

- Splay tree: se menciona como una estructura “elegante” que garantiza un comportamiento eficiente amortizado tanto para accesos como para actualizaciones.
- Árboles balanceados y con peso: el enfoque principal de este artículo son estructuras que son eficientes específicamente en el peor caso.

##### Resumen visual: La analogía del ahorro

Para que lo recuerdes fácilmente en tus clases:

- Peor caso ($Worst-case$): es como si tuvieras que pagar cada factura el día que llega, necesitas tener el dinero exacto en ese momento.
- Amortizado ($Amortized$): Es como ahorrar un poco cada mes para pagar una gran factura anual, el gasto “promedio” mensual es bajo, aunque el pago final sea grande.

##### Áreas principales de aplicación

- Organización física de bases de datos: para estructurar cómo se guardan y consultan los registros en el almacenamiento físico, permitiendo búsquedas rápidas por múltiples criterios (como buscar por nombre, edad y ciudad al mismo tiempo).
- Recuperación de información: fundamental para motores de búsqueda o sistema de gestión documental donde se necesita filtrar grandes volúmenes de datos basados en diversos atributos.
- Estructuras de archivos auto-organizadles: archivos que “aprenden” de su uso. Si accedes mucho a ciertos datos la estructura se ajusta sola para que los datos estén “más a mano”, optimizando el tiempo de respuesta.
- Geometría computacional: crucial para manejar coordenadas espaciales, detectar colisiones en robótica o procesar imágenes médicas.

##### Formas de Implementación

El texto menciona que estas estructuras pueden usarse de tres maneras:

- Como estructuras independientes: aplicadas directamente para resolver un problema de búsqueda específico.
- Como esquemas subyacentes: sirven como la “base” o el moto interno sobre el cual se construyen estructuras de datos aún más complejas.
- En formas mejoradas: adaptadas y potenciadas para aplicaciones de alta complejidad que requieren un rendimiento superior.

##### El desafío: recuperación asociativa

- El problema principal es al recuperación multiclave o asociativa, que consiste en buscar registros usando múltiples atributos a la vez.
- Debido a que las empresas y ejecutivos necesitan tomar decisiones rápidas, la base de datos requieren una organización física que permita respuestas veloces y actualizaciones eficientes.

##### Deficiencias de Estructuras Tradicionales

El texto menciona que varias estructuras de archivos conocidas tienen problemas cuando la base de datos es altamente dinámica (cambia mucho) y se requieren consultas de rango (buscar valores entre X e Y):

- Archivos invertidos (Inverted files): se determinó que tienen el peor rendimiento en este contexto.
- Grid files: son muy buenos en ciertos aspectos, pero no en todos.
- MDBT y kB-trees: Son alternativas que buscan superar estas limitaciones

##### La solución: árboles multidimensionales balanceados

- Se propone el uso de una estructura de árbol multi-camino multidimensional balanceada (como los kB-trees).
- Estas estructuras pueden mejorarse añadiendo punteros adicionales para servir como una organización de archivos dinámica y eficiente para la recuperación asociativa.

##### Comparativa de rendimiento

El artículo cita estudios que comparan estas estructuras:

- Ganador en versatilidad: los kB-trees y los grid files se disputan el mejor rendimiento dependiendo del tipo de operación específica que se realice.
- Perdedor: el archivo invertido queda rezagado en eficiencia para este tipo de consultas dinámicas multidimensionales.

El kB-tree es básicamente una evolución del B-tree que ya conoces, pero diseñada para manejar múltiples “llaves” o atributos al mismo tiempo sin perder el balanceo que lo hace rápido.

##### El TRIE y el problema del espacio

Un TRIE es una estructura donde las palabras se recuperan letra por letra (como se ve en la figura).

- Implementación clásica (con arreglos): es muy rápida (el tiempo depende solo del largo de la palabra), pero desperdicia mucho espacio de almacenamiento, ya que cada nodo debe tener espacio para todas las letras posibles del alfabeto, aunque no se usen.

##### La solución: árboles multidireccionales balanceados

El artículo propone usar un árbol de búsqueda multidimensional balanceado para implementar el TRIE. Esto ofrece ventajas críticas:

- Eficiencia de espacio: el almacenamiento ahora es proporcional al número total de caracteres en las palabras, eliminando el desperdicio de los arreglos vacíos.
- Rendimiento óptimo: permite buscar, insertar y borrar en un tiempo proporcional a: Longitud de la palabra + $log(n)$, donde n es el número total de palabras.

![](/private/var/folders/38/k7fql2x55qq127jbry_wpszr0000gn/T/TemporaryItems/com.apple.Photos.NSItemProvider/uuid=1DA51119-FE15-4577-B7D7-B90C284FCFE7&code=001&library=1&type=1&mode=1&loc=true&cap=true.png/Imagen 24-03-26 a las 7.43 a. m..png)

##### Estructuras de Archivos Auto-organizables

Este punto es clave para la eficiencia en sistemas que cambian constantemente:

- **Adaptación al uso:** Si los "pesos" de los datos se interpretan como la **frecuencia de acceso**, un árbol balanceado con peso se convierte en una estructura que se ajusta sola según cómo se use.
- **Optimización sin contadores:** Lo más increíble es que se mantiene en una forma casi óptima **sin necesidad de información previa** sobre las frecuencias y sin usar contadores internos para registrarlas.
- **Uso en bases de datos:** Es ideal para que los registros más consultados sean siempre los más rápidos de encontrar, mejorando el rendimiento general del sistema.

##### Geometría Computacional

Esta es quizás el área con problemas más complejos (gráficos 2D/3D y diseño de circuitos VLSI):

- **Estructuras Estáticas:** Menciona los **Interval trees** (árboles de intervalos) y **Segment trees** (árboles de segmentos). Tradicionalmente son estructuras estáticas basadas en árboles binarios completamente balanceados.
- **Dinamización:** El artículo explica que estas estructuras pueden volverse **dinámicas** (permitiendo cambios) si se usa un árbol balanceado con peso como base.
- **El caso de los D-trees:** Se han usado para dinamizar estos árboles de intervalos, pero el texto advierte que los **D-trees** son difíciles de usar porque sus algoritmos de actualización requieren operaciones de reestructuración muy complejas.

#### 4. Árboles multidimensionales y ponderados equilibrados: una mirada cercana a la literatura

##### Clasificación General

Además de los árboles balanceados, se introducen los **semi-balanceados**:

- **Acceso:** Tiempo logarítmico en el **peor caso**.
- **Actualización:** Tiempo logarítmico solo en el **caso amortizado**.

Existen dos enfoques principales para descubrir estas estructuras:

- **(a)** Extender una estructura de árbol balanceado existente.
- **(b)** Generalizar un concepto de balanceo.

##### Extender una Estructura

El ejemplo principal aquí son los **D-trees** (desarrollados por Mehlhorn):

- **Idea clave:** Un elemento con mucho "peso" se representa mediante **varios nodos hoja** para mantener la restricción de balanceo.
- **Ventajas:** Soportan acceso, inserción, eliminación y cambios de peso (*promote/demote*) en tiempo logarítmico en el peor caso.
- **Desventajas:** Requieren almacenamiento no lineal o algoritmos de reestructuración muy complicados.

##### Generalizar un Concepto de Balanceo

Este enfoque toma conceptos de balanceo de una dimensión (como los de los B-trees o AVL) y los lleva a las dimensiones k:

- **kB-trees y kB+-trees:** Son potentes pero muy complejos. Usan operaciones de reestructuración difíciles de implementar como *"Balancing with an indirect brother"* (balanceo con un hermano indirecto).
- **Árboles AVL Multidimensionales:** El autor los destaca positivamente porque:
  - Heredan las propiedades de los árboles AVL clásicos.
  - **Simplicidad:** Sus algoritmos de inserción y borrado son casi iguales a los de un AVL normal.
  - **Eficiencia:** Una inserción requiere, como máximo, **una sola operación de reestructuración** (rotación).

#### 5. Paradigmas para modelar árboles multidimensionales equilibrados y ponderados

##### El Concepto de "Violación de Estructura"

En un árbol balanceado normal (unidimensional), si un nodo está muy vacío o el árbol es muy alto, se reequilibra inmediatamente. En este paradigma multidimensional, se permiten estas **"violaciones de estructura"** (nodos con menos llaves de lo normal o ramas más largas), siempre que un **subárbol EQSON** de una dimensión inferior las "apoye".

##### Las Tres Estrategias de Apoyo

Para que el árbol siga siendo rápido (O(logn)), estas violaciones deben gestionarse con una de estas estrategias:

| Estrategia       | ¿Cómo funciona?                                    | Rendimiento (Actualización) |
| ---------------- | -------------------------------------------------- | --------------------------- |
| Local-support    | La violación es apoyada por subárboles “hermanos”. | Logarítmico amortizado      |
| Global-support   | Apoyada por todos los subárboles adyacentes.       | Logarítmico en peor caso.   |
| Neighbor-support | Apoyada por al menos un subárbol adyacente.        | Logarítmico en peor caso.   |

##### Estrategia Global-Support para kB-Trees

Los **kB-trees** son una generalización de los B-trees (como el árbol 2-3 de la **Fig. 4**):

- En un **2B-tree** (2 dimensiones), un nodo puede tener menos llaves de las permitidas si sus subárboles EQSON (que guardan datos de la siguiente dimensión) tienen una altura suficiente para "compensar" ese vacío.
- En la **Fig. 4b**, los nodos con una "X" son nodos vacíos (violaciones), pero el árbol es válido porque los subárboles B y C (de dimensión inferior) les dan soporte.

##### Estrategia Neighbor-Support para kAVL-Trees

Aquí se aplican los conceptos a los **árboles AVL multidimensionales**:

- Una **cadena de dos o más nodos unarios** (nodos con un solo hijo, marcados con "X" en la **Fig. 5b**) se considera una violación de estructura.
- En un **kAVL-tree**, esto es legal si al menos uno de los subárboles EQSON adyacentes tiene una altura de h−1 o más (donde h es la altura del nodo en la cadena).
- **Nota importante:** Un AVL de una sola dimensión (1AVL) **no** puede tener violaciones, porque no tiene subárboles de dimensiones inferiores para apoyarlas.

**Resumen simplificado:** Imagina que el árbol es un edificio. En una dimensión, cada piso debe estar perfectamente alineado. En múltiples dimensiones (este paradigma), permitimos que un piso sea "débil" o esté incompleto, siempre que tenga una "columna de soporte" (un subárbol EQSON de menor dimensión) que evite que el edificio (la eficiencia de búsqueda) se caiga.

#### 6. Árboles de atributos múltiples (MAT)

##### ¿Qué es un MAT?

El **MAT** es una estructura de datos diseñada para manejar registros con múltiples atributos de forma jerárquica.

- **Propiedad Clave:** Los nodos que tienen valores de atributos "cercanos" se agrupan juntos (*clustering*).
- **Estructura Recursiva:** Un MAT de k dimensiones tiene una raíz en el nivel 0. Sus hijos son MATs de (k−1)dimensiones que contienen registros con el mismo valor para el primer atributo (A1).
- **Orden:** Los nodos hijos de cualquier raíz (llamados *filial set*) siempre están ordenados de forma ascendente.

##### Construcción y Geometría

Como se ve en la **Fig. 6**, el MAT se construye:

1. Ordenando los registros por todos sus atributos.
2. Combinando elementos con el mismo valor de atributo en un solo nodo de forma recursiva.

- **Visión Geométrica:** Cada sub-MAT puede verse como un conjunto de puntos que yacen en un **hiperplano** del espacio de datos (por ejemplo, el plano donde el atributo A1=valor).

##### Variantes y Comparaciones

El texto compara el MAT con otras estructuras similares:

- **Modified DCT (Doubly Chained Tree):** Es más lento. El camino de búsqueda puede ser de hasta N+k nodos, mientras que en el MAT es solo k.
- **BST-complex:** Usa árboles AVL para mantener los conjuntos de hijos. Es muy bueno para coincidencias exactas pero malo para consultas de rango.
- **MDBT vs. kB-tree:** * El **MDBT** se balancea basándose solo en un atributo por nivel.
  - El **kB-tree** se balancea considerando todos los componentes, lo que garantiza una profundidad menor y mejor mantenimiento en entornos dinámicos (**Fig. 9**).

##### Generación del Directorio

Para que el MAT funcione en **memoria secundaria** (discos), el árbol se "aplana" o linealiza para formar un directorio.

- **Linealización:** Permite guardar el árbol como una tabla. Hay dos tipos principales (**Fig. 10**):
  - **Depth-first (Profundidad):** Mantiene juntos los campos de un mismo registro. Es excelente para consultas de **coincidencia total**.
  - **Breadth-first (Amplitud):** Mantiene juntos todos los nodos de un conjunto de hijos (*filial set*). Es el método ideal para **consultas de rango y coincidencia parcial**.

**Resumen práctico para tu curso:** El MAT es especialmente potente para bases de datos masivas en disco porque permite cargar solo las "páginas" necesarias. Si vas a buscar por rangos, la organización por **amplitud** (breadth-first) es la que deberías elegir según el autor.

##### Generación del Directorio y Algoritmos

Para que el **MAT** sea útil en discos (memoria secundaria), se debe transformar en un directorio lineal.

- **Campos del Registro:** Cada elemento del directorio guarda su valor, punteros al primer/último hijo (para amplitud) y punteros a "hermanos" y "primos" (para profundidad).
- **Costo de Generación:** El costo principal es el **ordenamiento inicial** de los datos. Si los datos están en RAM, la complejidad es O(kNlog(kN)). Si están en disco, el costo se mide en accesos a páginas: O(mlogm).

##### Algoritmos de Búsqueda

El concepto clave aquí es el **"Descenso Jerárquico"**: procesar la consulta nivel por nivel, de la raíz hacia abajo.

- **Coincidencia Total (Complete Match):** * Buscas un valor específico para cada atributo.
  - En cada nivel solo hay **un nodo** que cumple, por lo que haces una búsqueda binaria en cada conjunto de hijos (*filial set*).
  - **Complejidad:** O(logN). Es tan eficiente como un árbol binario estándar.
- **Coincidencia Parcial (Partial Match):**
  - Buscas valores para algunos atributos (ej: "todos los Fabricio de 23 años, no importa el distrito").
  - Si un nivel no está especificado, "calificas" a todos los hijos.
  - **Complejidad:** O(N1−t/klog(Nt/k)), donde t es el número de atributos especificados. A más filtros pongas, más rápido será.
- **Consultas de Rango (Range Query):**
  - Buscas valores dentro de un intervalo [li,hi].
  - Geométricamente, esto es buscar puntos dentro de un **hiperrectángulo** en un espacio de k dimensiones.
  - **Complejidad promedio:** O(ak−1log(N1/k)+K1), donde a es el número de nodos calificados por nivel y K1 es el número de resultados encontrados.

##### La ventaja del Almacenamiento Secundario

El artículo enfatiza que el **MAT** brilla cuando la base de datos está en el disco:

- **Clustering:** Al agrupar nodos relacionados en las mismas páginas del disco, se reduce drásticamente el número de veces que el sistema debe leer el disco (que es lo más lento en computación).
- **Breadth-first (Amplitud):** Se confirma como la mejor organización para búsquedas de rango porque mantiene los conjuntos de hijos juntos en el almacenamiento físico.

**Resumen Ejecutivo:** El MAT es una estructura jerárquica que, mediante una buena "linealización", logra que buscar en una base de datos de millones de registros sea casi tan rápido como buscar en una pequeña, especialmente si usas la organización por **amplitud**.

##### El Factor p (Fracción Calificada)

El rendimiento de una consulta de rango depende del valor de **p**, que representa qué fracción de los nodos de un conjunto cumple con la consulta.

- **Idea clave:** Cuanto más pequeño sea p (es decir, cuanto más específica sea tu búsqueda), mejor se comporta el MAT.
- En la **Tabla I**, verás que si p es una constante pequeña (c/N), el costo de acceso a memoria secundaria se vuelve bajísimo: **O(log(N1/k))**.

##### Comparación de Rendimiento

Esta es la parte fundamental para tus exámenes. El artículo clasifica las estructuras en 4 tipos según su costo de búsqueda de rango:

| Estructura               | Preprocesamiento | Almacenamiento | Búsqueda de Rango     |
| ------------------------ | ---------------- | -------------- | --------------------- |
| **MAT**                  | O(NlogN)         | O(N)           | O(N1−1/klog(N1/k)+K1) |
| **k-d tree / Quadtree**  | O(NlogN)         | O(N)           | O(N1−1/k+K1)          |
| **Range Tree**           | O(Nlogk−1N)      | O(Nlogk−1N)    | O(logkN+K1)           |
| **Overlapping k-ranges** | O(N1+ϵ)          | O(N1+ϵ)        | **O(logN+K1)**        |

##### Conclusiones de la Comparativa

- **El compromiso (Trade-off):** El artículo deja claro que **no existe la estructura perfecta**. Si quieres una búsqueda ultra rápida (como en *Overlapping k-ranges*), tendrás que pagar un costo altísimo en almacenamiento y tiempo de creación (preprocesamiento).
- **MAT vs. k-d trees:** Tienen complejidades muy similares en el peor caso. Sin embargo, el MAT es superior cuando los datos están en **memoria secundaria** (discos), gracias a que mantiene los nodos relacionados juntos en las mismas páginas físicas.
- **Búsqueda Secuencial:** Es el peor método (O(N)), solo útil si tienes poquísimos datos.

RESUMEN: El **MAT** se sitúa en un "punto dulce": tiene un costo de almacenamiento bajo y un tiempo de creación razonable, pero ofrece búsquedas de rango mucho más potentes que una simple lista o un árbol básico, especialmente para **bases de datos masivas** y **procesamiento de imágenes**.

#### 7. Paradigms for the MAT Data Structure

##### El Impacto del Orden de Atributos

Aunque el MAT se comporta como un *k-d tree*, su rendimiento en la vida real mejora drásticamente si se eligen bien las propiedades de los datos. El perfil del árbol (cómo se ramifica) depende directamente de en qué orden pongas los atributos (A1,A2, etc.).

##### Paradigma 1: La Regla del "Clustering" (Agrupamiento)

- **Concepto:** Un MAT es mucho más eficiente cuando los datos están "agrupados" (*large clusters*) que cuando están dispersos.
- **La Estrategia:** Se debe ordenar los atributos de tal manera que el tamaño promedio de los conjuntos de hijos (*filial sets*) disminuya a medida que bajas en el árbol.
- **Resultado:** Esto genera un árbol más **compacto**. Como se ve en las **Figuras 12 y 13**, intercambiar el orden de A1 y A2 puede hacer que el árbol tenga menos ramas pero más pobladas, lo cual es óptimo para búsquedas de rango.

##### Paradigma 2: La Regla de la Frecuencia y el Filtrado

Esta regla es fundamental para consultas de coincidencia parcial (*partial match*):

- **Frecuencia:** Los atributos que aparecen con más frecuencia en las consultas deben colocarse en los **niveles superiores** (niveles 1, 2...).
- **Filtrado:** Al poner los atributos más comunes arriba, se hace un "cribado" o filtrado inicial más fuerte. Esto reduce drásticamente el número de nodos que el algoritmo tiene que revisar en los niveles inferiores.
- **Para Rangos:** Para búsquedas de rango, coloca arriba los atributos que tengan más probabilidades de descartar la mayor cantidad de datos (los que resulten en menos nodos calificados).

RESUMEN: Para diseñar el MAT perfecto, sigue estos dos consejos:

1. **Compactación:** Organiza los datos para que el árbol no sea demasiado "ancho" en la base.
2. **Jerarquía de importancia:** Pon los filtros más importantes o frecuentes cerca de la raíz.

Estas estrategias confirman al MAT como una de las estructuras de indexación más prometedoras para sistemas de **recuperación de información**.

#### 8. Conclusiones

##### Resumen de Aportes

El artículo enfatiza que el diseño de estructuras multidimensionales es vital para áreas como **robótica, geometría computacional y bases de datos físicas**. Los cuatro pilares que se presentaron fueron:

1. La necesidad de árboles de búsqueda **auto-organizables** y balanceados para aplicaciones en **tiempo real**.
2. Un análisis de cómo extender los árboles balanceados tradicionales a dominios multidimensionales.
3. La creación de **paradigmas claros** para modelar árboles con peso y multidimensionales.
4. La descripción detallada del **MAT (Multiple Attribute Tree)** y su excelente rendimiento en consultas de rango.

##### Niveles de Paradigmas

El autor separa lo aprendido en dos niveles:

- **Nivel Abstracto:** Se enfoca en estrategias para soportar "violaciones de estructura" y explotar las propiedades del MAT.
- **Nivel de Implementación:** Aquí es donde entran las estrategias que vimos antes (**Local-support, Global-support y Neighbor-support**) para asegurar que el árbol funcione bien en la práctica.

##### La Lección Final

El texto cierra con una verdad fundamental en las Ciencias de la Computación: **"No existe un método universal para diseñar estructuras de datos multidimensionales eficientes"**.

El "truco" o la habilidad del ingeniero reside en:

- Identificar correctamente la formulación del problema computacional.
- Realizar una **descomposición apropiada** para una implementación eficiente.

> **Reflexión:** Aunque las nociones de complejidad de tiempo y espacio (O(logN), etc.) son útiles, a veces es difícil mapear los costos reales debido a la complejidad de las computadoras de alta velocidad.
