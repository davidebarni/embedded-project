# Torretta mitragliatrice spara dardi
Autore: Davide Barni
# Componenti utilizzati: 
* 1x Arduino Uno
* 1x IR Receive module
* 1x Telecomando Remote Control
* 1x Ultrasonic sensor
* 1x LCD1602 Module
* 1x I2C adattatore seriale per LCD display
* 5x Led IR emettitore e ricevitore
* 1x Led rosso
* 1x Fotoresistenza
* 13x Resistenze (x5 2K, x6 1K, x1 330, x1 220)
* 1x Alimentatore 9V 3A
* 2x Mini motori DC da 3-6 V e 0,2 A
* 1x Power supply module
* 1x Batteria 9v
* 3x Servomotori 4,8 V
* 1x L293D chip ponte H
# Descrizione del progetto:
Il progetto consiste nella realizzazione di una torretta mitragliatrice che spara proiettili in plastica, la torretta è posizionata a terra su 3 piedini e può girarsi tramite due servomotori sull'asse delle x (su se stessa) per un angolo di max 180 gradi e sull'asse delle y (in altezza) in modo da poter calibrare il tiro in maniera efficace e precisa inoltre presenta una canna dove i proiettili con un terzo servomotore vengono spinti fino ad arrivare a due motori che tramite rotazione faranno partire il proiettile.
Alla base è presente un display che visualizza la modalità attualmente in uso e il numero di colpi rimanenti.
Sul corpo della torretta sono posizionati una serie di sensori che permettono tre modalità di utilizzo, la prima è una modalità standard in cui l’utente ha il controllo completo della torretta mentre nelle ultime due modalità la torretta agisce in maniera autonoma e intelligente senza l’ausilio dell’utente.
L’utente può uscire in qualsiasi momento da una modalità ed effettuare lo switch tra una modalità e l’altra.
# Features e Modalità:
* Modalità manuale: è controllata dall’utente tramite un telecomando a infrarossi
* Modalità ultrasensore: tramite il sensore ultrasonico la torretta scansiona il suo angolo di visione per trovare un possibile ostacolo/nemico e colpirlo
* Modalità tracking (fonte di calore): tramite dei LED IR posizionati è in grado di tracciare e seguire un bersaglio e colpirlo
* Funzione ricarica munizioni: è possibile ricaricare la torretta inserendo i proiettili in plastica ed è in grado di contare il numero di proiettili inseriti così da tenere sempre aggiornato il numero di proiettili attualmente presenti nel caricatore.
# Foto e breve funzionamento
<img src="https://user-images.githubusercontent.com/105540548/168422712-6ec13b6c-9042-4a1e-95c7-2cbdfa925937.jpg" width="500">
<img src="https://user-images.githubusercontent.com/105540548/168422716-ab71859b-5e9e-417b-a34d-7d8f9426abe9.jpg" width="500">
<img src="https://user-images.githubusercontent.com/105540548/168422717-ae1a94ed-c4cf-492e-938e-d7754121052a.jpg"  width="500">
<img src="https://github.com/davidebarni/embedded-project/blob/main/Shoot.gif" alt="animated" style="max-width: 100%;">

