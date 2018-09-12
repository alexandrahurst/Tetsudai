import sqlite3
import xml.dom.minidom

# create sqlite db
conn = sqlite3.connect('JMdict_e_TEST.sqlite')
c = conn.cursor()

c.execute('''CREATE TABLE "entry" (
	"ent_seq" INTEGER NOT NULL UNIQUE,
	PRIMARY KEY(ent_seq)
)''')

c.execute('''CREATE TABLE "k_ele" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"keb" TEXT NOT NULL,
	"entry" INTEGER NOT NULL,
	FOREIGN KEY("entry") REFERENCES entry(ent_seq)
)''')
c.execute('''CREATE TABLE "ke_inf" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"k_ele" INTEGER NOT NULL,
	FOREIGN KEY ("k_ele") REFERENCES k_ele(id)
)''')
c.execute('''CREATE TABLE "ke_pri" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"k_ele" INTEGER NOT NULL,
	FOREIGN KEY ("k_ele") REFERENCES k_ele(id)
)''')

c.execute('''CREATE TABLE "r_ele" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"reb" TEXT NOT NULL,
	"re_nokanji" INTEGER NOT NULL,
	"entry" INTEGER NOT NULL,
	FOREIGN KEY("entry") REFERENCES entry(ent_seq)
)''')
c.execute('''CREATE TABLE "re_restr" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"r_ele" INTEGER NOT NULL,
	FOREIGN KEY ("data") REFERENCES k_ele(id),
	FOREIGN KEY ("r_ele") REFERENCES r_ele(id)
)''')
c.execute('''CREATE TABLE "re_inf" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"r_ele" INTEGER NOT NULL,
	FOREIGN KEY ("r_ele") REFERENCES r_ele(id)
)''')
c.execute('''CREATE TABLE "re_pri" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"r_ele" INTEGER NOT NULL,
	FOREIGN KEY ("r_ele") REFERENCES r_ele(id)
)''')

c.execute('''CREATE TABLE "sense" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"entry" INTEGER NOT NULL,
	FOREIGN KEY("entry") REFERENCES entry(ent_seq)
)''')
c.execute('''CREATE TABLE "stagk" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "stagr" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "pos" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "xref" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "ant" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "field" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "misc" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "s_inf" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "lsource" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"lang" TEXT NOT NULL,
	"ls_type" TEXT NOT NULL,
	"ls_wasei" INTEGER NOT NULL,
	"term" TEXT,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "dial" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')
c.execute('''CREATE TABLE "gloss" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"lang" TEXT NOT NULL,
	"g_gend" TEXT,
	"pri" INTEGER NOT NULL,
	"term" TEXT NOT NULL,
	"sense" INTEGER NOT NULL,
	FOREIGN KEY ("sense") REFERENCES sense(id)
)''')

# read xml file and insert entries
dom = xml.dom.minidom.parse('JMdict_e')
entries = dom.getElementsByTagName('entry')

def getText (node):
    rc = []
    for childNode in node.childNodes:
        if childNode.nodeType == childNode.TEXT_NODE:
            rc.append(childNode.data)
    return ''.join(rc)

#def k_eleIdFromKebAndEntry (keb, entry):
#	c.execute("SELECT id FROM k_ele WHERE keb=? AND entry=?", (keb, entry))
#	return c.fetchone()[0]


for entry in entries:
	ent_seq = getText(entry.getElementsByTagName('ent_seq')[0])
	c.execute("INSERT INTO entry(ent_seq) VALUES (?)", (ent_seq,))
	
	
	for k_ele in entry.getElementsByTagName('k_ele'):
		keb = getText(k_ele.getElementsByTagName('keb')[0])
		c.execute("INSERT INTO k_ele(keb, entry) VALUES (?, ?)", (keb, ent_seq))
		k_eleId = c.lastrowid
		
		ke_infs = map(getText, k_ele.getElementsByTagName('ke_inf'))
		for ke_inf in ke_infs:
			c.execute("INSERT INTO ke_inf(data, k_ele) VALUES (?, ?)", (ke_inf, k_eleId))
			
		ke_pris = map(getText, k_ele.getElementsByTagName('ke_pri'))
		for ke_pri in ke_pris:
			c.execute("INSERT INTO ke_pri(data, k_ele) VALUES (?, ?)", (ke_pri, k_eleId))
			
			
	for r_ele in entry.getElementsByTagName('r_ele'):
		reb = getText(r_ele.getElementsByTagName('reb')[0])
		re_nokanji = len(r_ele.getElementsByTagName('re_nokanji')) != 0
		c.execute("INSERT INTO r_ele(reb, re_nokanji, entry) VALUES (?, ?, ?)", (reb, re_nokanji, ent_seq))
		r_eleId = c.lastrowid
		
		#re_restrs = map(lambda node:k_eleIdFromKebAndEntry(getText(node), ent_seq), r_ele.getElementsByTagName('re_restr'))
		re_restrs = map(getText, r_ele.getElementsByTagName('re_restr'))
		for re_restr in re_restrs:
			c.execute("INSERT INTO re_restr(data, r_ele) VALUES (?, ?)", (re_restr, r_eleId))
			
		re_infs = map(getText, r_ele.getElementsByTagName('re_inf'))
		for re_inf in re_infs:
			c.execute("INSERT INTO re_inf(data, r_ele) VALUES (?, ?)", (re_inf, r_eleId))
			
		re_pris = map(getText, r_ele.getElementsByTagName('re_pri'))
		for re_pri in re_pris:
			c.execute("INSERT INTO re_pri(data, r_ele) VALUES (?, ?)", (re_pri, r_eleId))
			
			
	for sense in entry.getElementsByTagName('sense'):
		c.execute("INSERT INTO sense(entry) VALUES (?)", (ent_seq,))
		senseId = c.lastrowid
		
		stagks = map(getText, sense.getElementsByTagName('stagk'))
		for stagk in stagks:
			c.execute("INSERT INTO stagk(data, sense) VALUES (?, ?)", (stagk, senseId))
			
		stagrs = map(getText, sense.getElementsByTagName('stagr'))
		for stagr in stagrs:
			c.execute("INSERT INTO stagr(data, sense) VALUES (?, ?)", (stagr, senseId))
			
		poss = map(getText, sense.getElementsByTagName('pos'))
		for pos in poss:
			c.execute("INSERT INTO pos(data, sense) VALUES (?, ?)", (pos, senseId))
			
		xrefs = map(getText, sense.getElementsByTagName('xref'))
		for xref in xrefs:
			c.execute("INSERT INTO xref(data, sense) VALUES (?, ?)", (xref, senseId))
			
		ants = map(getText, sense.getElementsByTagName('ant'))
		for ant in ants:
			c.execute("INSERT INTO ant(data, sense) VALUES (?, ?)", (ant, senseId))
			
		fields = map(getText, sense.getElementsByTagName('field'))
		for field in fields:
			c.execute("INSERT INTO field(data, sense) VALUES (?, ?)", (field, senseId))
			
		miscs = map(getText, sense.getElementsByTagName('misc'))
		for misc in miscs:
			c.execute("INSERT INTO misc(data, sense) VALUES (?, ?)", (misc, senseId))
			
		s_infs = map(getText, sense.getElementsByTagName('s_inf'))
		for s_inf in s_infs:
			c.execute("INSERT INTO s_inf(data, sense) VALUES (?, ?)", (s_inf, senseId))
			
		lsourceElems = sense.getElementsByTagName('lsource')
		for lsourceElem in lsourceElems:
			lang = 'eng'
			if lsourceElem.hasAttribute('xml:lang'):
				lang = lsourceElem.getAttribute('xml:lang')
			ls_type = 'full'
			if lsourceElem.hasAttribute('ls_type'):
				ls_type = lsourceElem.getAttribute('ls_type')
			ls_wasei = lsourceElem.hasAttribute('ls_wasei')
			term = None if len(lsourceElem.childNodes) == 0 else getText(lsourceElem)
			c.execute("INSERT INTO lsource(lang, ls_type, ls_wasei, term, sense) VALUES (?, ?, ?, ?, ?)", (lang, ls_type, ls_wasei, term, senseId))
				
		dials = map(getText, sense.getElementsByTagName('dial'))
		for dial in dials:
			c.execute("INSERT INTO dial(data, sense) VALUES (?, ?)", (dial, senseId))
			
		glossElems = sense.getElementsByTagName('gloss')
		for glossElem in glossElems:
			lang = 'eng'
			if glossElem.hasAttribute('xml:lang'):
				lang = glossElem.getAttribute('xml:lang')
			g_gend = None if not glossElem.hasAttribute('g_gend') else glossElem.getAttribute('g_gend')
			pri = len(glossElem.getElementsByTagName('pri')) != 0
			term = getText(glossElem)
			c.execute("INSERT INTO gloss(lang, g_gend, pri, term, sense) VALUES (?, ?, ?, ?, ?)", (lang, g_gend, pri, term, senseId))

# create indices for columns frequently used in WHERE clauses
c.execute("CREATE INDEX k_ele_entry_index ON k_ele (entry)")
c.execute("CREATE INDEX ke_inf_k_ele_index ON ke_inf(k_ele)")
c.execute("CREATE INDEX ke_pri_k_ele_index ON ke_pri(k_ele)")
c.execute("CREATE INDEX r_ele_entry_index ON r_ele(entry)")
c.execute("CREATE INDEX re_restr_r_ele_index ON re_restr(r_ele)")
c.execute("CREATE INDEX re_inf_r_ele_index ON re_inf(r_ele)")
c.execute("CREATE INDEX re_pri_r_ele_index ON re_pri(r_ele)")
c.execute("CREATE INDEX sense_entry_index ON sense(entry)")
c.execute("CREATE INDEX stagk_sense_index ON stagk(sense)")
c.execute("CREATE INDEX stagr_sense_index ON stagr(sense)")
c.execute("CREATE INDEX pos_sense_index ON pos(sense)")
c.execute("CREATE INDEX xref_sense_index ON xref(sense)")
c.execute("CREATE INDEX ant_sense_index ON ant(sense)")
c.execute("CREATE INDEX field_sense_index ON field(sense)")
c.execute("CREATE INDEX misc_sense_index ON misc(sense)")
c.execute("CREATE INDEX s_inf_sense_index ON s_inf(sense)")
c.execute("CREATE INDEX lsource_sense_index ON lsource(sense)")
c.execute("CREATE INDEX dial_sense_index ON dial(sense)")
c.execute("CREATE INDEX gloss_sense_index ON gloss(sense)")

# commit changes and close connection
conn.commit()
conn.close()