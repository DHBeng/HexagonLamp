sourceFile = open("WebVisu.html", "r")
htmlCode = sourceFile.readlines()
htmlForArduino = ""
for line in htmlCode:
    line = line.replace("    ", "")
    line = line.replace("\n", "")
    line = line.replace("\"", "\\\"")
    htmlForArduino = htmlForArduino + line
sourceFile.close()

destinatioFile = open("WebVisu.txt", "w")
codeLine = ""
lineStart = "client.println(\""
lineEnd = "\");"
j = 0
for i in htmlForArduino:
    if j == 0:
        codeLine = lineStart
    codeLine = codeLine + i
    j = j + 1
    if j >= 80:
        if (i == ">") or (i == "}"):
            codeLine = codeLine + lineEnd
            destinatioFile.write(codeLine + "\n")
            print(codeLine)
            j = 0
        else:
            j = j + 1
if j > 0:
    codeLine = codeLine + lineEnd
    destinatioFile.write(codeLine )
    print(codeLine)

destinatioFile.close()