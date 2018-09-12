Instructions for generating output resources:
1) freq.sqlite: Rename "Rikaisama_v22.9.3_Ubuntu.xpi" to 
   "Rikaisama_v22.9.3_Ubuntu.zip" and extract freq.sqlite from the "freq" 
   directory to "../res"
2) entities.tsv: Export "entities.gnumeric" as TSV to "../res" using 
   Gnumeric.
3) JMdict_e_TEST.sqlite: Run "jmdict-xml-to-sqlite.py", which converts 
   "JMdict_e" to "JMdict_e_TEST.sqlite", then copy to "../res".
4) KanjiStrokeOrders_v3.001.ttf: Copy "KanjiStrokeOrders_v3.001.ttf" to  
   "../res".
5) kanjidic2.sqlite: Extract "kanjidic2.xml.gz" to "kanjidic2.xml", then run 
   "kanjidic-xml-to-sqlite.py", which converts "kanjidic2.xml" to 
   "kanjidic2.sqlite", then copy to "../res".

