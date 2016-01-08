-- MySQL dump 10.13  Distrib 5.7.9, for Win32 (AMD64)
--
-- Host: 192.168.65.225    Database: ticketsystem
-- ------------------------------------------------------
-- Server version	5.6.26-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `categories`
--

DROP TABLE IF EXISTS `categories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `categories` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `categories`
--

LOCK TABLES `categories` WRITE;
/*!40000 ALTER TABLE `categories` DISABLE KEYS */;
INSERT INTO `categories` VALUES (1,'Finest'),(2,'Konstruktion'),(3,'Schachtel'),(4,'Postprozessor'),(5,'Hauptprogramm');
/*!40000 ALTER TABLE `categories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `comments`
--

DROP TABLE IF EXISTS `comments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `comments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `viewed` int(11) DEFAULT '0',
  `text` mediumtext,
  `date` datetime DEFAULT NULL,
  `from_user` int(11) NOT NULL,
  `to_user` int(11) DEFAULT NULL,
  `ticket` int(11) NOT NULL,
  `image` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_ticket_idx` (`ticket`),
  KEY `fk_user_idx` (`from_user`),
  KEY `fk_image_comment_idx` (`image`),
  CONSTRAINT `fk_image_comment` FOREIGN KEY (`image`) REFERENCES `img` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_ticket` FOREIGN KEY (`ticket`) REFERENCES `ticket` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=102 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `comments`
--

LOCK TABLES `comments` WRITE;
/*!40000 ALTER TABLE `comments` DISABLE KEYS */;
INSERT INTO `comments` VALUES (11,1,'Ein Kreis mit Radius = 0.001 wird als Kontur akzeptiert. Er wird nicht als \"Kontur mit minimaler Fläche\" angezeigt. \"','2016-01-06 15:18:02',3,1,9,NULL),(12,1,'Einträge Tech ändern : nach Datenbank dauert es eeeeewig !!!!','2016-01-06 15:19:05',3,1,10,NULL),(13,1,'Kontur-Info : Nach Ändern : UNDO geht nicht ? Soll das so ? ','2016-01-06 15:20:26',3,1,11,NULL),(14,1,'Unschön : Lösung : Gruppe aufheben bei Fahnenmanipulation !  - Wenn Kontur in Gruppe ist : Fahnenmanipulation geht für diese Kontur nicht. Wenn man die Gruppe auflöst : Dann geht alles.','2016-01-06 15:23:42',3,1,12,NULL),(15,1,'Wenn Fahne zu nahe an Kontur ist, wird trotzdem \"Fahne schneidet Kontur\" in Liste eingetragen. \nDa muss ein andere Text hin ! ','2016-01-06 15:24:42',3,2,13,NULL),(16,1,'Klären wenn Meldung \"FAhne zu nahe\" kommt !  Der Mindestabstand der Fahne von der Kontur kann innerhalb der aut. Rf nicht mehr geändert werden ! Das ist bei kleinen Konturen evtl. problematisch . ','2016-01-06 15:27:36',3,1,14,NULL),(17,1,'An offenen Konturen kann man Stege so setzen, dass sie über Anfang oder Ende der Kontur hinaus gehen. ','2016-01-06 15:28:16',3,2,15,NULL),(18,1,'In FFahneFehlerData::value wird unterschieden : Anscheinend kommt da nicht der richtige Fehlercode an. \n','2016-01-06 15:29:42',3,2,13,NULL),(19,1,'Wenn man die  Funktion gleich abbricht, ohne was zu ändern, dann sind in einigen der Innenkonturen die Fahnen unsichtbar. Die  Schleife mit \"setvisible\" aus dem mouseReleaseEvent bringt hier nix. \n','2016-01-06 15:31:26',3,1,16,NULL),(20,1,' Fahnenmanipulation : TAB-Taste lässt Fahne an der Maus. Man kann den Cursor nicht unten bei den  Parametern benutzen. ','2016-01-06 15:32:50',3,1,17,NULL),(21,1,'Zusätzliche Funktion (Finest 6 kann das nicht) : Beim Bohren sollen für Senken, Gewinde  usw. unterschiedliche Farben möglich sein. Das muss in das \"Zeichnen\" rein. In der Datenbank müssen da auch unterschiedliche Indices sein. Die Farben können für alle Arten des Bohrens vorerst die gleichen sein : Man muss es nur in der Datenbank ändern können. ','2016-01-06 15:34:28',3,2,18,NULL),(22,1,'Konturabschnitt bearbeiten : Das geht ohne Abstürze. ABER : Wenn man Anfangs-und End-Element ausgewählt hat UND dann was anders auswählen will , Z.B ein anders End-Element, weil man sich vertippt hat : Dann wird immer nur ein Element ausgewählt . Man kann also keinen Konturabschnitt mit 2 oder mehr Elementen auswählen. Es sieht so aus, als könnte man das erste Element nicht mehr auswählen.  ','2016-01-06 15:36:15',3,1,19,NULL),(23,1,'Aussenkontur mit mehrene Stegen. \"Kontur Stege löschen\" angeklickt : Steg sind weg. Funktion NICHT verlassen : Beim Zoomen wird die Auswahl und die Stege immer kurz sichtbar. ','2016-01-06 15:37:21',3,1,20,NULL),(24,1,'Aussenkontur mit mehrene Stegen . dann \"Steg löschen\" : Angekllickt  : Absturz. ','2016-01-06 15:38:20',3,1,21,NULL),(25,1,'Wenn man nach der aut.RF eine Innenkontur in eine Gruppe nimmt, dann z.B. \" Kopieren/Verschieben\" aufruft, dann ist die Gruppe \"halbiert\" : Die Löschelemente werden aktiviert und die sind nicht in der Gruppe. Kann man da was machen ? Z.B. bei Löschen Technologie auch die Gruppe löschen ? Bei Rotieren und Spiegeln tritt es auch auf. Beim Skalieren dagegen wird die Gruppe aufgelöst.\n','2016-01-06 15:39:01',3,2,22,NULL),(26,1,'Lösung: Falls das Gruppenflag eines der KonturElemente vom Typ(28,38,29,39) gesetzt ist wird beim Löschen der Technologie das Löschelement in die Gruppe übertragen.','2016-01-06 15:39:11',3,2,22,NULL),(27,1,'Nach der aut Rf wird bei offenen Konturen nicht mehr der Konturbeginn angezeigt. Man muss die Konturen erst identifizieren, bevor Anfang/Ende angezeigt werden. Bitte so machen, dass bei offenen Konturen (egal welches Werkzeug) immer Anfang (und/oder Ende) angezeigt werden. ','2016-01-06 15:40:58',3,1,23,NULL),(28,1,'Parallelen  : Die Anwender wollten immer die Funktionalität (weil sie das von Autocad oder so kennen) : Auf Gerade klicken, daneben klicken : Dann kommt dort die Parallele. Das gab es genau so in Finest 7. Warum wurde das geändert : Wir haben im Prinzip wieder den ALTERNATIVE-Button. Find ich nicht gut. ','2016-01-06 15:43:37',3,2,24,NULL),(29,1,'Unschön : Wenn NACH  dem Erzeugen von True-Type-Texten noch eine Strecke (oder Punkt oder Bogen) erzeugt wird, dann wird diese in der Farbe REFERENZ (weiss) gezeichnet. Das Werkzeug Referenz von TrueType bleibt also erhalten. ','2016-01-06 15:45:35',3,2,25,NULL),(30,1,'Datenmaske Auftrag : Bei Auswahl \"drehbar\" : +-180 : Besser : Zwangswinkel +-180\" : Da weiss man, auf was es sich bezieht.     ','2016-01-06 15:47:11',3,2,26,NULL),(31,1,'Makro : erweiterte Darstellung : Bei Doppelklick kommt das Bild dann als eigenes Widget  und liegt vor der Eingabemaske : kann man da was machen ? ','2016-01-06 15:47:54',3,1,27,NULL),(32,1,'Erinnerung : DXF : - Profile : Eintrag rausnehmen / Eintrag zufügen wäre schön. ','2016-01-06 15:48:58',3,2,28,NULL),(33,1,' Erinnerung : \"definiere aussen\" fehlt noch. Inn ktm0.txt ist es schon drin.','2016-01-06 15:50:05',3,1,29,NULL),(34,1,'Fehler : Während \"Gerade\" funktioniert \"zoom alles! nicht so recht ','2016-01-06 15:51:55',3,1,30,NULL),(35,1,'Fehler ListLabel : Bei den Aufträgen werden die Teile nicht gezeichnet (auch wenn man wie bei den Bauteilen zeichnet : Absturz !) ','2016-01-06 15:56:45',3,1,31,NULL),(36,1,'Fehler : Wenn Die Konstruktion aus einer Funktion heraus (z.B. \"Gerade\")  beendet wird, gibt es ein Problem mit der User-Behandlung : Absturz. ','2016-01-06 15:57:50',3,1,32,NULL),(37,1,'Fehler : Vermaßung anzeigen : Zoom-Fenster : Vermassungstexte werde nicht aufgebaut , wenn sie ausserhalb des BTs liegen.','2016-01-06 15:58:28',3,1,33,NULL),(38,1,'Nicht alle Funktionen reagieren auf ESC','2016-01-06 15:59:05',3,1,34,NULL),(39,1,'wenn eine Technologie ausgewählt ist, eine Platte angelegt oder ausgewählt un d man über Teile passend zur platte Teile auswählt wird st37 und die passende Dicke ausgewählt, selbst wenn man ein anderes Material in der Technologie wählt\n','2016-01-06 16:00:50',3,2,35,NULL),(40,0,'Liste der letzten Objekte : Alle Objekte kommen als NC-Codes zurück : Man kann also Teile und Pläne nicht mehr zum nochmaligen Verarbeiten aufrufen.  Beim Einlesen der \"Liste der letzen Objekte\"  aus der Datenbank darf die Tabellennummer nicht überschrieben werden : Denn an der Tabellennummer erkennt der PP ob es ein Plan, ein Teil oder ein NC-Code ist. ','2016-01-06 16:02:13',3,2,36,NULL),(41,1,'Probleme beim Klinken, einzelne Ecke ist problematisch','2016-01-06 16:03:38',3,1,37,NULL),(42,0,'AufgabeaUFRUF sCHACHTELFENSTER EINSTELLEN aBSTURZ','2016-01-06 16:04:44',3,2,38,NULL),(43,1,'mehrfachpos.1:\nauch wenn Schalter Stückzahlüberwachung gesetzt ist kann ich die Stückzahl größer einstellen als diegeforderte Stückzahl\n2. ist die eingestellte Stückzahl drastisch zu groß werden die Teile positioniert, das Originalteil fehlt aber','2016-01-06 16:05:47',3,2,39,NULL),(44,1,'hier bin ich mir nicht sicher ob das ganze jetzt überfordere:\n-horizontal denke ich alles ok\nVertikal Zeichenabstand kann nicht geändert werden\nIch denke die TTS sind ein Kapitel für sich über das man reden muss','2016-01-06 16:06:37',3,2,40,NULL),(45,1,'grundsätzlich schön gelöst.\nwenn ich 3 Punkte habe die 3 Punkte anklicken fertig - prima \nABER ich habe einen Punkt und muss die beiden anderen mit der Maus etc eingeben, dann muss ich jeden dieser Punkte mit CR bestätigen, hier fehlt ein Hinweis auf CR oder ein übernehmen','2016-01-06 16:07:53',3,1,41,NULL),(46,1,'Vorschlag Vermaßung bearbeiten, Beispiel teil im zoom vermaßt, hilfslinien zu dicht am Teil, Maßhilfslinien weiter nach aussen ziehen','2016-01-06 16:10:54',3,1,42,NULL),(47,1,'Vorschlag Winkelvermaßung zwischen zwei geraden','2016-01-06 16:12:12',4,1,43,NULL),(48,1,'Raster einstellbar und als Konstruktionshilfe nutzen','2016-01-06 16:12:45',4,1,44,NULL),(49,1,'Vorschlag, da die rechte Maustaste ohne Funktion ist kann man dochüber einen Befehlsabbruch oder eine Befehlswiederholung analog ACAD nachdenken  (7.1)','2016-01-06 16:13:33',4,1,45,NULL),(50,1,'viele der Makros sind kundenspezifisch','2016-01-06 16:13:57',4,1,46,NULL),(51,1,'Makrokatalog Schalter ob Vollbild oder nicht','2016-01-06 16:14:40',4,1,47,NULL),(52,1,'Bei Funktionen wie KonturInfo oder TeileInfo stürzt das Programm ab falls ein neues Feature gestartet wird ohne das alte zu beenden. \n\nAbsturz in FEFMaskeManager::buttonVisible( );','2016-01-06 16:22:03',2,1,48,NULL),(53,1,'Ich kann es bei mir nicht reproduzieren, daher wird auf Testen gesetzt.','2016-01-06 16:31:44',1,2,48,NULL),(54,0,'Ich habe dies getestet un keinen Fehler feststellen können. Es werden alle von mir erzeugten Kreise mit Radisu 0.001 mm als \"kritische Konturen\" angezeigt.','2016-01-07 09:50:35',2,2,9,NULL),(55,0,'Ich habe dies getestet un keinen Fehler feststellen können. Es werden alle von mir erzeugten Kreise mit Radisu 0.001 mm als \"kritische Konturen\" angezeigt.\n','2016-01-07 09:50:54',2,3,9,NULL),(56,1,'Bei den Tests auf meinem Rechner sind die Anschnittfahnen, die zu nah an der Kontur sind, alle korrekt angezeigt worden. Diese Meldung erscheint im Übrigen nur, wenn die Option \"Fahnen kürzen\" nicht angehakt ist. ','2016-01-07 10:01:06',2,2,13,NULL),(57,0,'Bei den Tests auf meinem Rechner sind die Anschnittfahnen, die zu nah an der Kontur sind, alle korrekt angezeigt worden. Diese Meldung erscheint im Übrigen nur, wenn die Option \"Fahnen kürzen\" nicht angehakt ist. ','2016-01-07 10:01:24',2,3,13,NULL),(58,0,'Haben wir beide getestet und nicht reproduzieren können. Ich setze den Status wieder auf \"Testen\", so dass du den Eintrag auf \"Geschlossen\" setzen kannst.','2016-01-07 10:07:03',2,3,10,NULL),(59,0,'War nur ein Bug, jetzt geht es.','2016-01-07 11:15:57',1,3,11,NULL),(66,0,'Absturz war reproduzierbar. Liste der selektierten Elemente wurde nicht gelöscht, das Element selbst aber schon.','2016-01-07 15:44:25',2,3,21,NULL),(72,0,'Derselbe Fehler bei Ticket 21, daher schon gelöst.','2016-01-07 18:29:20',1,3,20,NULL),(73,0,'Neuer Status: Testen','2016-01-07 18:29:25',1,3,20,NULL),(87,0,'Neuer Status: Testen','2016-01-08 09:30:27',1,4,41,NULL),(88,1,'Ticket erstellt.','2016-01-08 10:22:35',1,1,55,NULL),(89,0,'Beim erzeugen und aktualisieren von Teilelisten aktuelles Datum der Bauteil in XML speichern (Tabelle PLAN_TBL)','2016-01-08 10:24:27',1,2,55,NULL),(90,0,'Liste anzeigen aller  Bauteilen mit einem jüngeren Datum als von der Teileliste.\nAnzuzeigende Felder Variable (BAUTEIL_TBL).','2016-01-08 10:29:02',1,2,55,NULL),(91,1,'Ticket erstellt.','2016-01-08 10:31:50',1,1,56,NULL),(92,1,'Nach Größe soriteiren: umschreibendes Rechteckt.','2016-01-08 10:31:50',1,1,56,NULL),(93,0,'Austeigen und absteigend sortieren','2016-01-08 10:33:31',1,2,55,NULL),(94,1,'Ticket erstellt.','2016-01-08 10:53:05',1,1,57,NULL),(95,1,'Ticket erstellt.','2016-01-08 11:32:38',1,1,58,NULL),(96,0,'Ich kann es in der aktuellen Version nicht reproduzieren.\nBitte testen ob der Bug noch drin ist.','2016-01-08 11:52:45',1,3,32,NULL),(97,0,'Neuer Status: Testen','2016-01-08 11:53:00',1,3,32,NULL),(98,0,'Was ist genau hier gemeint?\nBei mir kann ich auf \"Strecke\" gehen, den ersten Punkt anklicken und dann auf \"zoom alles\" gehen, es funktioniert.','2016-01-08 11:56:11',1,3,30,NULL),(99,0,'Neuer Status: Testen','2016-01-08 11:56:13',1,3,30,NULL),(100,0,'Neuer Status: Testen','2016-01-08 11:58:40',1,3,33,NULL),(101,0,'Neuer Status: Testen','2016-01-08 12:06:38',1,4,37,NULL);
/*!40000 ALTER TABLE `comments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `img`
--

DROP TABLE IF EXISTS `img`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `img` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `img` blob NOT NULL,
  `ticket` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_ticket_id_idx` (`ticket`),
  CONSTRAINT `fk_ticket_id` FOREIGN KEY (`ticket`) REFERENCES `ticket` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `img`
--

LOCK TABLES `img` WRITE;
/*!40000 ALTER TABLE `img` DISABLE KEYS */;
/*!40000 ALTER TABLE `img` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `priority`
--

DROP TABLE IF EXISTS `priority`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `priority` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `number` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `number_UNIQUE` (`number`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `priority`
--

LOCK TABLES `priority` WRITE;
/*!40000 ALTER TABLE `priority` DISABLE KEYS */;
INSERT INTO `priority` VALUES (1,'Normal',5),(2,'Wichtig',6),(3,'Sehr wichtig',7),(4,'Niedrig',3);
/*!40000 ALTER TABLE `priority` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `projects`
--

DROP TABLE IF EXISTS `projects`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `projects` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `projects`
--

LOCK TABLES `projects` WRITE;
/*!40000 ALTER TABLE `projects` DISABLE KEYS */;
INSERT INTO `projects` VALUES (1,'Finest');
/*!40000 ALTER TABLE `projects` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `state`
--

DROP TABLE IF EXISTS `state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `state` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `state`
--

LOCK TABLES `state` WRITE;
/*!40000 ALTER TABLE `state` DISABLE KEYS */;
INSERT INTO `state` VALUES (1,'Offen'),(2,'Testen'),(3,'Geschloßen');
/*!40000 ALTER TABLE `state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ticket`
--

DROP TABLE IF EXISTS `ticket`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ticket` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) DEFAULT NULL,
  `from_user` int(11) DEFAULT NULL,
  `to_user` int(11) DEFAULT NULL,
  `project` int(11) DEFAULT NULL,
  `state` int(11) DEFAULT NULL,
  `categorie` int(11) DEFAULT NULL,
  `priority` int(11) DEFAULT NULL,
  `version` varchar(45) DEFAULT NULL,
  `percent_complete` int(11) DEFAULT NULL,
  `title` varchar(500) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `processed_by` int(11) DEFAULT NULL,
  `processed_on` datetime DEFAULT NULL,
  `tested_by` int(11) DEFAULT NULL,
  `tested_on` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_from_user_idx` (`from_user`),
  KEY `fk_to_user_idx` (`to_user`),
  KEY `fk_tested_by_idx` (`tested_by`),
  KEY `fk_processed_by_idx` (`processed_by`),
  KEY `fk_project` (`project`),
  KEY `fk_categorie` (`categorie`),
  KEY `fk_state` (`state`),
  KEY `fk_priority` (`priority`),
  KEY `fk_type` (`type`),
  CONSTRAINT `fk_categorie` FOREIGN KEY (`categorie`) REFERENCES `categories` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_from_user` FOREIGN KEY (`from_user`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_priority` FOREIGN KEY (`priority`) REFERENCES `priority` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_processed_by` FOREIGN KEY (`processed_by`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_project` FOREIGN KEY (`project`) REFERENCES `projects` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_state` FOREIGN KEY (`state`) REFERENCES `state` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_tested_by` FOREIGN KEY (`tested_by`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_to_user` FOREIGN KEY (`to_user`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_type` FOREIGN KEY (`type`) REFERENCES `tickettype` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=59 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ticket`
--

LOCK TABLES `ticket` WRITE;
/*!40000 ALTER TABLE `ticket` DISABLE KEYS */;
INSERT INTO `ticket` VALUES (9,1,3,3,1,2,2,1,NULL,NULL,'Ein Kreis mit Radius = 0.001 wird als Kontur akzeptiert.','2016-01-06 15:18:02',NULL,NULL,NULL,NULL),(10,1,3,3,1,2,5,1,NULL,NULL,'Einträge Tech ändern : nach Datenbank dauert es eeeeewig !!!!','2016-01-06 15:19:05',NULL,NULL,NULL,NULL),(11,1,3,3,1,2,2,1,NULL,NULL,'Kontur-Info : Nach Ändern : UNDO geht nicht ? Soll das so ? ','2016-01-06 15:20:26',NULL,NULL,NULL,NULL),(12,1,3,1,1,1,2,4,NULL,NULL,'Unschön : Lösung : Gruppe aufheben bei Fahnenmanipulation ! ','2016-01-06 15:23:42',1,NULL,NULL,NULL),(13,1,3,3,1,2,2,1,NULL,NULL,'Wenn Fahne zu nahe an Kontur ist, wird trotzdem \"Fahne schneidet Kontur\" in Liste eingetragen','2016-01-06 15:24:42',NULL,NULL,NULL,NULL),(14,3,3,2,1,1,2,1,NULL,NULL,'Klären wenn Meldung \"FAhne zu nahe\" kommt !','2016-01-06 15:27:36',NULL,NULL,NULL,NULL),(15,1,3,2,1,1,2,1,NULL,NULL,'Stege : ','2016-01-06 15:28:16',NULL,NULL,NULL,NULL),(16,1,3,3,1,2,2,2,NULL,NULL,' Länge aller Fahnen ändern : Problemchen bei Abbruch. ','2016-01-06 15:31:26',NULL,NULL,NULL,NULL),(17,1,3,1,1,1,2,1,NULL,NULL,'Fahnenmanipulation : TAB-Taste lässt Fahne an der Maus. ','2016-01-06 15:32:50',NULL,NULL,NULL,NULL),(18,1,3,2,1,1,2,1,NULL,NULL,'Unterschiedliche Farben bei Bohren : Andere Darstellung, gleiche Farbe ','2016-01-06 15:34:28',NULL,NULL,NULL,NULL),(19,1,3,3,1,2,2,2,NULL,NULL,'Fehler : Parallelkontur :','2016-01-06 15:36:15',1,NULL,NULL,NULL),(20,1,3,3,1,2,2,1,NULL,NULL,'Kontur Stege löschen : Unsauber. ','2016-01-06 15:37:21',1,NULL,NULL,NULL),(21,1,3,3,1,2,2,1,NULL,NULL,'Steg löschen : Absturz.','2016-01-06 15:38:20',NULL,NULL,NULL,NULL),(22,1,3,2,1,1,2,1,NULL,NULL,'unschön : Gruppe und Löschelemente ','2016-01-06 15:39:01',NULL,NULL,NULL,NULL),(23,1,3,1,1,1,2,1,NULL,NULL,' Nach der aut Rf wird bei offenen Konturen nicht mehr der Konturbeginn angezeigt.','2016-01-06 15:40:58',NULL,NULL,NULL,NULL),(24,3,3,2,1,1,2,1,NULL,NULL,'Parallelen  : Die Anwender wollten immer die Funktionalität (weil sie das von Autocad oder so kennen) ','2016-01-06 15:43:37',NULL,NULL,NULL,NULL),(25,1,3,2,1,1,2,4,NULL,NULL,'Unschön : Wenn NACH  dem Erzeugen von True-Type-Texten noch eine Strecke (oder Punkt oder Bogen) erzeugt wird, dann wird diese in der Farbe REFERENZ (weiss) gezeichnet.','2016-01-06 15:45:35',NULL,NULL,NULL,NULL),(26,1,3,2,1,1,1,4,NULL,NULL,'Datenmaske Auftrag : Bei Auswahl \"drehbar\" : +-180 : Besser : Zwangswinkel +-180\"','2016-01-06 15:47:11',NULL,NULL,NULL,NULL),(27,1,3,1,1,1,2,1,NULL,NULL,'unschön : Makro : ','2016-01-06 15:47:54',NULL,NULL,NULL,NULL),(28,1,3,2,1,1,2,4,NULL,NULL,'Erinnerung : DXF : - Profile : Eintrag rausnehmen / Eintrag zufügen wäre schön.  ','2016-01-06 15:48:58',NULL,NULL,NULL,NULL),(29,2,3,1,1,1,2,1,NULL,NULL,' Erinnerung : \"definiere aussen\" fehlt noch. Inn ktm0.txt ist es schon drin. ','2016-01-06 15:50:05',NULL,NULL,NULL,NULL),(30,1,3,3,1,2,2,1,NULL,NULL,'Fehler : Während \"Gerade\" funktioniert \"zoom alles! nicht so recht ','2016-01-06 15:51:55',1,NULL,NULL,NULL),(31,1,3,1,1,1,1,1,NULL,NULL,'Fehler ListLabel : Bei den Aufträgen werden die Teile nicht gezeichnet','2016-01-06 15:56:45',NULL,NULL,NULL,NULL),(32,1,3,3,1,2,2,3,NULL,NULL,'Absturz in der Konstruktion','2016-01-06 15:57:50',1,NULL,NULL,NULL),(33,1,3,3,1,2,2,1,NULL,NULL,'Fehler : Vermaßung anzeigen : Zoom-Fenster','2016-01-06 15:58:28',1,NULL,NULL,NULL),(34,1,4,1,1,1,1,1,NULL,NULL,'Nicht alle Funktionen reagieren auf ESC','2016-01-06 15:59:05',NULL,NULL,NULL,NULL),(35,1,4,2,1,1,3,1,NULL,NULL,'Fehler bei der Teileauswahl','2016-01-06 16:00:50',NULL,NULL,NULL,NULL),(36,1,3,3,1,2,4,1,NULL,NULL,'Liste der letzten Objekte : Alle Objekte kommen als NC-Codes zurück','2016-01-06 16:02:13',NULL,NULL,NULL,NULL),(37,1,4,4,1,2,2,1,NULL,NULL,'Probleme beim Klinken, einzelne Ecke ist problematisch','2016-01-06 16:03:38',1,NULL,NULL,NULL),(38,1,4,4,1,2,3,1,NULL,NULL,'AufgabeaUFRUF sCHACHTELFENSTER EINSTELLEN aBSTURZ','2016-01-06 16:04:44',NULL,NULL,NULL,NULL),(39,1,4,2,1,1,3,1,NULL,NULL,'Problem beim Schachteln:','2016-01-06 16:05:47',NULL,NULL,NULL,NULL),(40,1,4,2,1,1,2,1,NULL,NULL,'True type Schriften','2016-01-06 16:06:37',NULL,NULL,NULL,NULL),(41,3,4,4,1,2,2,1,NULL,NULL,'Kreis über drei Punkte','2016-01-06 16:07:53',1,NULL,NULL,NULL),(42,4,4,1,1,1,2,1,NULL,NULL,'Vorschlag Vermaßung bearbeiten','2016-01-06 16:10:54',NULL,NULL,NULL,NULL),(43,4,4,1,1,1,2,1,NULL,NULL,'Vorschlag Winkelvermaßung zwischen zwei geraden','2016-01-06 16:12:12',NULL,NULL,NULL,NULL),(44,4,4,1,1,1,2,1,NULL,NULL,'Raster einstellbar und als Konstruktionshilfe nutzen','2016-01-06 16:12:45',NULL,NULL,NULL,NULL),(45,4,4,1,1,1,1,1,NULL,NULL,'Vorschlag: rechte Maustaste','2016-01-06 16:13:33',NULL,NULL,NULL,NULL),(46,4,4,3,1,1,2,1,NULL,NULL,'viele der Makros sind kundenspezifisch','2016-01-06 16:13:56',NULL,NULL,NULL,NULL),(47,4,4,1,1,1,2,1,NULL,NULL,'Makrokatalog Schalter ob Vollbild oder nicht','2016-01-06 16:14:40',NULL,NULL,NULL,NULL),(48,1,2,2,1,3,2,3,NULL,NULL,'Absturz bei Features ohne \"Maske\" ','2016-01-06 16:22:03',1,NULL,2,NULL),(55,2,1,1,1,1,3,1,NULL,NULL,'Teile aktualisieren','2016-01-08 10:22:35',NULL,NULL,NULL,NULL),(56,2,1,1,1,1,3,1,NULL,NULL,'Teileliste sortieren','2016-01-08 10:31:50',NULL,NULL,NULL,NULL),(57,2,1,1,1,1,3,1,NULL,NULL,'Bauteile in der Konstruktion öffnen','2016-01-08 10:53:04',NULL,NULL,NULL,NULL),(58,2,1,1,1,1,3,1,NULL,NULL,'Bauteil aus der Teileliste löschen','2016-01-08 11:32:38',NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `ticket` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tickettype`
--

DROP TABLE IF EXISTS `tickettype`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tickettype` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tickettype`
--

LOCK TABLES `tickettype` WRITE;
/*!40000 ALTER TABLE `tickettype` DISABLE KEYS */;
INSERT INTO `tickettype` VALUES (1,'Bug'),(2,'Feature'),(3,'Kärung'),(4,'Vorschlag'),(5,'Aufgabe');
/*!40000 ALTER TABLE `tickettype` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `email` varchar(100) DEFAULT NULL,
  `pwd` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (1,'Aziz','ghannami@cam-concept.com','e65ac9372a656094f1fd6f409f28183624883fc5'),(2,'Bernhard',NULL,'0126900a8e0b3a4ca252ce135f6083413ee82854'),(3,'Rolf',NULL,'77aa1026e89be0ca81722a3920e4a32c7960775f'),(4,'Norman',NULL,'4864ccb4939929874a71c5255d77f90846dede54');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-08 13:07:46
