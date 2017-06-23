// file:///I:/MyProjects/PB_ESP8266_WiFiConfig_Vyvoj/doc/_testy/s?_s=SSID1&_p=heslo&_n=jmeno_zarizeni&_a=on&_st=on&_i=192.168.1.1&_m=255.255.255.0&_g=192.168.1.3&_d=192.168.1.3&{n}={v}
// Hlavicka stranky, skript, ikony
static const char PAGE_INDEX1[] PROGMEM = R"=====(
<!DOCTYPE html><html lang="cs"><head><meta charset="utf-8" name="viewport" content="width=device-width, initial-scale=1"/><title>xPablo.cz Setup</title>
<style>div,input {margin-bottom: 5px;}body{width:230px;display:block;margin-left:auto;margin-right:auto;}
#lock {
   background-image: url("data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==");
   width: 16px;
   height: 16px;
   background-size: cover;
   display: inline-block;
   margin: 3px 0 0 5px;
}

input[type="checkbox"]:checked ~ .reveal-if-active {
  opacity: 1;
  max-height: 200px;
  overflow: visible;
}

.reveal-if-active {
  opacity: 0;
  max-height: 0;
  overflow: hidden;
  transform: scale(1.0);
  transition: 0.5s;
  input[type="checkbox"]:checked ~ & {
    opacity: 1;
    max-height: 200px;
    overflow: visible;
    transform: scale(1);
  }
}
body {font-family:"Geneva CE","Arial CE",sans-serif;}
</style>
<script>function c(l){document.getElementById('_s').value=l.innerText||l.textContent;document.getElementById('_p').focus();}</script>
</head><body><h1>ESP8266 WiFiConfig</h1>
)=====";

/* Polozky nalezenych SSID
{v} - SSID
{a} - RSSI (kvalita signalu v %)
{s} - bud nic, nebo <img id="lock">
*/
static const char SSID_ITEM[] PROGMEM = R"=====(
<div><a href='#' onclick='c(this)'>{v}</a>({a}){s}</div>)=====";

/* Zacatek formulare pro vyplneni
{s} - SSID
{p} - password
{n} - Netbios name
{a} - AP mode (checkbox)
{c} - staticka IP konfigurace (checkbox)
{i} - ip adresa
{m} - maska
{g} - brana
{d} - dns server
*/
static const char PAGE_INDEX2[] PROGMEM = R"=====(
<form method='post' action='s'>
<label>SSID<br><input id='_s' name='_s' length=32 required {s}></label><br>
<label>Heslo<br><input id='_p' name='_p' length=64 {p} type='password'></label><br>
<label>Jméno zařízení<br><input name='_n' length=32 pattern='^[a-zA-Z][a-zA-Z0-9-_\\.]{1,32}$' title='Jmeno dle NetBios konvence (max. 16 znaku pismena, cisla a znaky "_.")' {n}></label><br>
<label><input name='_a' type='checkbox' {a}>Režim AP</label><br>
<input name='_st' type='checkbox' {c}><label>Statická IP konfigurace</label><br>
<div class="reveal-if-active">
<label>IP adresa<br><input type="text" name="_i" pattern='((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$' title='ip adresa ve formatu <cislo>.<cislo>.<cislo>.<cislo>' value='{i}'></label><br>
<label>Síťová maska<br><input type="text" name="_m" pattern='((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$' title='ip adresa ve formatu <cislo>.<cislo>.<cislo>.<cislo>' value='{m}'></label><br>
<label>Síťová brána<br><input type="text" name="_g" pattern='((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$' title='ip adresa ve formatu <cislo>.<cislo>.<cislo>.<cislo>' value='{g}'></label><br>
<label>Server DNS<br><input type="text" name="_d" pattern='((^|\.)((25[0-5])|(2[0-4]\d)|(1\d\d)|([1-9]?\d))){4}$' title='ip adresa ve formatu <cislo>.<cislo>.<cislo>.<cislo>' value='{d}'></label><br>
</div>
)=====";

// Uzivatelsky pridane polozky
static const char PAGE_PARAM_HDR[] PROGMEM = R"=====(
<hr>
<h2>Uživatelské položky</h2>
)=====";

static const char PAGE_PARAM[] PROGMEM = R"=====(
<label>{t}<br><input name='{n}' length='{l}' value='{v}'></label><br>
)=====";

// Konec formulare a stranky
static const char PAGE_END[] PROGMEM = R"=====(
<input type='submit' value='Uložit a restartovat'></form>
</body></html>
)=====";

// Nenalezeno zadne SSID
static const char PAGE_NO_SSID[] PROGMEM = R"=====(
<div>Nenalezená žádná síť. Občerstvi stránku pro nové hledání.</div>
)=====";

// Konfigurace ulozena - restartuji...
static const char PAGE_SAVED[] PROGMEM = R"=====(
<!DOCTYPE html><html lang="cs"><head><meta http-equiv="refresh" content="10;url=/"/>
<meta charset="utf-8" name="viewport" content="width=device-width, initial-scale=1"/>
<title>xPablo Setup - Konfigurace uložena</title>
<style>div,input {margin-bottom: 5px;}body{width:2š0px;display:block;margin-left:auto;margin-right:auto;}</style>
</head><body>
Uloženo do EEPROM...<br/>
Restart za 10 sekund.
</body></html>
)=====";

static const char PAGE_CAPTIVEPORTALCATCH[] PROGMEM = R"=====(
<!DOCTYPE html><html lang="cs"><head><meta charset="utf-8" name="viewport" content="width=device-width, initial-scale=1"/><title>xPablo Setup - Přesměrování</title>
<style>div,input {margin-bottom: 5px;}body{width:200px;display:block;margin-left:auto;margin-right:auto;}</style>
</head><body>
Pro nastaveni klikni na <a href="{v}"><button>Nastaveni</button></a>
</body></html>
)=====";
