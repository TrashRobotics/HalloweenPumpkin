import re
fileName = 'test.raw'

name = "sound"  #re.split('\.|/', fileName)[-2]

fin = open(fileName, 'rb')
fout = open(name + ".h", "wt")

audioLen = 0
audioData = '\t'

count = 0
byte = fin.read(1)
while byte:
    audioData += hex(ord(byte)) + ', '
    audioLen += 1
    count += 1
    if count >= 8:
        audioData += "\n\t"
        count = 0
    byte = fin.read(1)

header = """
#define {nameUpper}_LENGTH {audioLen}

const uint8_t {name}_data[] PROGMEM = {{
{audioData}
}};
""".format(nameUpper=name.upper(), name=name, audioLen=audioLen, audioData=audioData)

print(header)
fout.write(header)
fin.close()
fout.close()
