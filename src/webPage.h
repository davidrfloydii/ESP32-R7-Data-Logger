#pragma once

namespace WebPage
{
    String getWebPageContent()
    {
        String html = "";

        html += "<!DOCTYPE html>\n";
        html += "<html>\n";
        html += "<head>\n";
        html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
        html += "<title>R7 Datalogger</title>\n";
        html += "<style>\n";
        html += "html {\n";
        html += "    font-family: Arial, Helvetica, sans-serif;\n";
        html += "}\n";
        html += "body {\n";
        html += "    background-color: #f4f4f4;\n";
        html += "    margin: 0;\n";
        html += "    padding: 0;\n";
        html += "}\n";
        html += ".container {\n";
        html += "    max-width: 800px;\n";
        html += "    margin: 50px auto;\n";
        html += "    text-align: center;\n";
        html += "}\n";
        html += "h1 {\n";
        html += "    color: #333;\n";
        html += "}\n";
        html += ".button {\n";
        html += "    display: inline-block;\n";
        html += "    padding: 10px 20px;\n";
        html += "    margin: 10px;\n";
        html += "    font-size: 16px;\n";
        html += "    border: none;\n";
        html += "    border-radius: 5px;\n";
        html += "    cursor: pointer;\n";
        html += "    transition-duration: 0.4s;\n";
        html += "}\n";
        html += ".button-data {\n";
        html += "    background-color: #858585;\n";
        html += "    color: #fff;\n";
        html += "}\n";
        html += ".button-delete {\n";
        html += "    background-color: #780320;\n";
        html += "    color: #fff;\n";
        html += "}\n";
        html += ".button:hover {\n";
        html += "    background-color: #0056b3;\n";
        html += "}\n";
        html += "</style>\n";
        html += "</head>\n";
        html += "<body>\n";
        html += "<div class=\"container\">\n";
        html += "    <h1>R7 Datalogger</h1>\n";
        html += "    <a href=\"view-data\"><button class=\"button button-data\">View Data</button></a>\n";
        html += "    <a href=\"download\"><button class=\"button button-data\">Download Data</button></a>\n";
        html += "    <a href=\"delete\"><button class=\"button button-delete\">Delete Data</button></a>\n";
        html += "</div>\n";
        html += "</body>\n";
        html += "</html>\n";

        return html;
    }

} // namespace WebPage