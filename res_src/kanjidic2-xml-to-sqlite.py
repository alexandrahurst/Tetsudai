import sqlite3
import xml.dom.minidom

# create sqlite db
conn = sqlite3.connect('kanjidic2.sqlite')
c = conn.cursor()

c.execute('''CREATE TABLE "kanji" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"literal" TEXT NOT NULL
)''')

c.execute('''CREATE TABLE "reading" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"kanji" INTEGER NOT NULL,
	FOREIGN KEY("kanji") REFERENCES kanji(id)
)''')
c.execute('''CREATE TABLE "meaning" (
	"id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"data" TEXT NOT NULL,
	"kanji" INTEGER NOT NULL,
	FOREIGN KEY ("kanji") REFERENCES kanji(id)
)''')

# read xml file and insert entries
dom = xml.dom.minidom.parse('kanjidic2.xml')
characters = dom.getElementsByTagName('character')

def getText (node):
	rc = []
	for childNode in node.childNodes:
		if childNode.nodeType == childNode.TEXT_NODE:
			rc.append(childNode.data)
	return ''.join(rc)

for character in characters:
	literal = getText(character.getElementsByTagName('literal')[0])
	c.execute("INSERT INTO kanji(literal) VALUES (?)", (literal,))
	kanjiId = c.lastrowid
	
	reading_meaning = character.getElementsByTagName('reading_meaning')
	if len(reading_meaning) > 0:
		for rmgroup in reading_meaning[0].getElementsByTagName('rmgroup'):
			for reading in rmgroup.getElementsByTagName('reading'):
				if reading.getAttribute('r_type').startswith('ja_'):
					c.execute("INSERT INTO reading(data, kanji) VALUES (?, ?)", (getText(reading), kanjiId))
			for meaning in rmgroup.getElementsByTagName('meaning'):
				if not meaning.hasAttribute('m_lang') or meaning.getAttribute('m_lang') == 'en':
					c.execute("INSERT INTO meaning(data, kanji) VALUES (?, ?)", (getText(meaning), kanjiId))

# create indices for columns frequently used in WHERE clauses
c.execute("CREATE INDEX reading_kanji ON reading(kanji)")
c.execute("CREATE INDEX meaning_kanji ON meaning(kanji)")

# commit changes and close connection
conn.commit()
conn.close()