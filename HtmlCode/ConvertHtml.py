def insertValue(destination, line: str, value: str, hexValue: bool):
    valueStartPos = 0
    temp = ""
    
    valueStartPos = line.find("\"temp")
    if (hexValue):
        temp = "client.print(\"" + line[0:valueStartPos+1] +"#\");\n"
    else:
        temp = "client.print(\"" + line[0:valueStartPos+1] +"\");\n"
    destination.write(temp)
    temp = "client.print(" + value + ");\n"
    destination.write(temp)
    temp = "client.println(\"" + line[valueStartPos+5:] + "\");\n"
    destination.write(temp)    
    return

def find_nth(string: str, subString : str, n: int):
    start = string.find(subString)
    while start >= 0 and n > 1:
        start = string.find(subString, start+len(subString))
        n -= 1
    return start

def buildSwitchCase(destination, options: str):
    numberOptions = options.count("<option")
    option = ""

    temp = "switch (WebVisu::mode)\n"
    destination.write(temp) 
    temp = "{\n"
    destination.write(temp)

    for i in range(1, numberOptions + 1):
        temp = "case " + str(i) + ":\n"
        destination.write(temp)
        for j in range(1, numberOptions + 1):
            if (j != numberOptions):
                option = options[find_nth(options, "<option", j):find_nth(options, "<option", j + 1)]
            else:
                option = options[find_nth(options, "<option", j):]
            if (i == j):
                temp = "\tclient.println(\"" + option[0:8] + "selected " + option[8:] + "\");\n"
                destination.write(temp)
            else:
                temp = "\tclient.println(\"" + option + "\");\n"
                destination.write(temp)
        temp = "\tbreak;\n"
        destination.write(temp)
    temp = "}\n"
    destination.write(temp)
    
    return


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
htmlLine = ""
j = 0
for i in htmlForArduino:
    htmlLine = htmlLine + i
    j = j + 1
    if j >= 80:
        if (i == ">") or (i == "}"):
            if (htmlLine.find("<select") >= 0):
                if (htmlLine.endswith("</select>")):
                    selectedPos = htmlLine.find("<select")
                    optionPos = htmlLine.find("<option")
                    line = "client.println(\"" + htmlLine[0:selectedPos] + "\");\n"
                    destinatioFile.write(line)
                    line = "client.println(\"" + htmlLine[selectedPos:optionPos] + "\");\n"
                    destinatioFile.write(line)
                    buildSwitchCase(destinatioFile, htmlLine[optionPos:])
                    j = 0
                    htmlLine = ""
                else:
                    j = j + 1
            elif (htmlLine.find("name") >= 0 and htmlLine.find("temp") >= 0):
                if (htmlLine.find("name=\\\"M") >= 0):
                    modeCnt = htmlForArduino.count("<option")
                    insertValue(destinatioFile, htmlLine, "WebVisu::mode", False)
                elif (htmlLine.find("name=\\\"B") >= 0):
                    insertValue(destinatioFile, htmlLine, "WebVisu::brightness", False)
                elif (htmlLine.find("name=\\\"S") >= 0):
                    insertValue(destinatioFile, htmlLine, "WebVisu::speed", False)
                elif (htmlLine.find("name=\\\"C") >= 0):
                    insertValue(destinatioFile, htmlLine, "WebVisu::colorHex, HEX", True)
                else:
                    print("!!! Error: Input not defind !!!")
                j = 0
                htmlLine = ""
            else:
                htmlLine = "client.println(\"" + htmlLine + "\");\n"
                destinatioFile.write(htmlLine)
                j = 0
                htmlLine = ""
        else:
            j = j + 1
            
if j > 0:
    htmlLine = "client.println(\"" + htmlLine + "\");\n"
    destinatioFile.write(htmlLine )

print("--- File created ---")

destinatioFile.close()