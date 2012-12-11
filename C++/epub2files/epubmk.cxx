//******************************************************************************
// Author: Josh Wainwright                                                     *
// Date: 30/11/2012                                                            *
// File Number: 08                                                             *
//                                                                             *
// Description: Various functions that create the final epub document. These   *
//              are all static and are generally used only once.               *
//******************************************************************************

#include <iostream>
#include <sstream>
#include <fstream>

#include "epubmk.h"
#include "epubfunctions.h"

Meta mkcontent(Meta metadata){
    // mkcontent ---------------------------------------------------------------
    metadata.clear("content");

    //Switch for setting the Unique User Identifier depending on if the user is 
    // on a Linux system or not
    if(metadata.get_os()){
        //uuidgen is a 
        system("uuidgen > temp");
        ifstream temp("temp");
        string uuid;

        getline (temp,uuid);
        metadata.set_uuid(uuid);
        system("rm temp");
    }else{
        metadata.set_uuid("00000000-0000-0000-0000-000000000000");
    }

    metadata.set_content(
        "<?xml version=\"1.0\" ?>\n"
        "<package version=\"2.0\" xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"" + metadata.get("id") + "\">\n"
        "<metadata xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:opf=\"http://www.idpf.org/2007/opf\">\n\n"
        "\t<meta name=\"cover\" content=\"BookCover\"/>\n"
        "\t\t<dc:title>\"" + metadata.get("title") + "\"</dc:title>\n"
        "\t\t<dc:language>\"" + metadata.get("lang") + "\"</dc:language>\n"
        "\t\t<dc:identifier id=\"" + metadata.get("id") + "\" opf:scheme=\"uuid\">urn:uuid:" + metadata.get("uuid") + "</dc:identifier>\n"
        "\t\t<dc:creator>\"" + metadata.get("author") + "\"</dc:creator>\n"
        "\t\t<dc:publisher>\"" + metadata.get("publish") + "\"</dc:publisher>\n"
        //could also add subject, discription, editor. All optional.
        "\t\t<dc:date>\"" + metadata.get("date") + "\"</dc:date>\n" 
        "\t</metadata>\n\n"
        "\t<manifest>\n"
        "\t\t<!--HTML Files-->\n"
    );

    stringstream ss;//create a stringstream
    for (int i = 0; i < metadata.get_files_size(); i++){
        ss.str("");
        ss << i+1;//add number to the stream

        string filename;
        filename = catfile(metadata.get_file(i));
        metadata.set_content(
            "\t\t<item id=\"html" + ss.str() + "\" media-type= \"application/xhtml+xml\" href=\"" + filename + "\"></item>\n"
        );
    } 

    metadata.set_content("\n\t\t<!--Image Files-->\n");

    for (int i = 0; i < metadata.get_images_size(); i++){
        ss.str("");
        ss << i+1;//add number to the stream
        ss.str();

        string imagename;
        imagename = catfile(metadata.get_image(i));
        //FOR SOME REASON, WHEN THIS IS REMOVED, THE IMAGES WORK
        //metadata.set_content(
        //    "\t\t<item id=\"fig" + ss.str() + "\"media-type= image/gif\" href=\"" + imagename + "\"></item>\n"
        //);
    } 

    metadata.set_content(
        "\n\t\t<item id=\"MyNcx\" media-type= \"application/x-dtbncx+xml\" href=\"toc.ncx\"></item>\n"
    );

    //Chooser for BOOK  COVER
    metadata.set_content(
        "\n\t\t<item id=\"BookCover\" media-type= \"image/jpg\" href=\"cover.jpg\"></item>\n"
    );

    //<!-- Cover page [optional] -->
    metadata.set_content(
        "\t\t<item id=\"cp\" media-type= \"application/xhtml+xml\" href=\"coverpg.html\"></item>\n"
        "\t</manifest>\n\n"
        "\t<spine toc=\"MyNcx\">\n"
        // "\t\t<itemref idref=\"cp\"/>\n"
        // "\t\t<itemref idref=\"tc\"/>\n"
    );

    for(int i=0;i<metadata.get_files_size();i++){
        ss.str("");
        ss << i+1;
        string numb = ss.str();
        metadata.set_content("\t\t<itemref idref=\"html" + numb + "\"/>\n");
    }

    metadata.set_content(
        "\t</spine>\n\n"
        "\t<guide>\n"
        "\t\t<reference type=\"cover\" title=\"Cover\" href=\"coverpg.html\"></reference>\n"
        "\t\t<reference type=\"toc\" title=\"Table of Contents\" href=\"toc.html\"></reference>\n"
        "\t\t<reference type=\"text\" title=\"Beginning\" href=\"intro.html\"></reference>\n"
        "\t</guide>\n\n"
        "</package>"
    );
    return metadata;
}

Meta mkcontainer(Meta metadata){
// mkcontainer -------------------------------------------------------
    metadata.clear("container");
    
    metadata.set_container(
        "<?xml version=\"1.0\"?>\n"
        "<container version=\"1.0\" \n"
        "xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\n"
        "\t<rootfiles>\n"
        "\t\t<rootfile full-path=\"OEPBS/content.opf\" media-type=\"application/oebps-package+xml\"/>\n"
        "\t</rootfiles>\n"
        "</container>"
    );

    return metadata;
}

Meta mktoc(Meta metadata){
    //mktoc-------------------------------------------------------------
    metadata.clear("toc");

    metadata.set_toc(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        "<!DOCTYPE ncx PUBLIC \"-//NISO//DTD ncx 2005-1//EN\" \"http://www.daisy.org/z3986/2005/ncx-2005-1.dtd\">\n"
        "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"en\">\n"
        "\t<head>\n"
        //<!-- The content of dtb:uid must be exactly the same as the uuid
        // specified in the OPF file. -->
        "\t\t<meta name=\"dtb:uid\" content=\"urn:uuid:" + metadata.get("uuid") + "\"/>\n"
        //<!-- The NCX specification allows for a hierarchal table of contents. 
        // It is recommended that the depth be set at 1 since that is all that 
        // the Kindle and Nook support. -->
        "\t\t<meta name=\"dtb:depth\" content=\"1\"/>\n"
        //<!-- The total page count and the max page number are not needed. 
        // Leave them set to zero. -->
        "\t\t<meta name=\"dtb:totalPageCount\" content=\"0\"/>\n"
        "\t\t<meta name=\"dtb:maxPageNumber\" content=\"0\"/>\n"
        "\t</head>\n"
        "\t<docTitle><text>" + metadata.get("title") + "</text></docTitle>\n"
        "\t<docAuthor><text>" + metadata.get("author") + "</text></docAuthor>\n"
        //<!-- The navMap section is the heart of the NCX file. For EPUB eBooks 
        // this section specifies the entries in the navigational table of 
        // contents. For the older keyboard Kindles this section defines a set 
        // of predefined bookmarks. You can jump between the preset bookmarks by
        // pressing the right or left arrows of the 5-way controller. It appears
        // that the Kindle Touch and the Kindle Fire do not use the NCX file. 
        // The order of the entries is specified by the playOrder attribute. The
        // location is specified by the content src attribute. This consists of 
        // an HTML file name such as "filename.html" or an HTML filename with a
        // named anchor added such as "filename.html#mark". The preset bookmarks
        // for the Kindle will appear as little dots on the progress bar at the
        // bottom of the screen.-->
        "\t<navMap>\n"
    );
    
    stringstream ss;//create a stringstream
    for(int i=0;i<metadata.get_files_size();i++){
        ss.str("");
        ss << i+1;
        string numb = ss.str();
        metadata.set_toc(
            "\t\t<navPoint id=\"navpoint-" + numb + "\" playOrder=\"" + numb + "\">\n"
            "\t\t\t<navLabel><text>file" + numb + "</text></navLabel>\n"
            "\t\t\t<content src=\"" + catfile(metadata.get_file(i)) + "\"/>\n"
            "\t\t</navPoint>\n"
        );
    }

    metadata.set_toc(
        "\t</navMap>\n"
        "</ncx>"
    );
    return metadata;
}

void mkstructure(Meta metadata){
    struct stat sb;

    string folder = metadata.get("title_under") + "/";
    if(!(stat(folder.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
        string mkdir = "mkdir " + metadata.get("title_under") + "/";
        system(mkdir.c_str());
    }
    string mimetype = metadata.get("title_under") + "/mimetype";
    ofstream mimetype2(mimetype.c_str());
    mimetype2 << "application/epub+zip";
    mimetype2.close();

    string meta = metadata.get("title_under") + "/META-INF";
    if(!(stat(meta.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
        string meta2 = "mkdir " + metadata.get("title_under") + "/META-INF";
        system(meta2.c_str());
    }

    string oepbs = metadata.get("title_under") + "/OEPBS";
    if(!(stat(oepbs.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))){
        string oepbs2 = "mkdir " + metadata.get("title_under") + "/OEPBS";
        system(oepbs2.c_str());
    }
    
    string toc_ncx = metadata.get("title_under") + "/OEPBS";
    string toc_ncx2 = toc_ncx + "/toc.ncx";
    ofstream toc_ncx3(toc_ncx2.c_str());
    toc_ncx3 << metadata.get("toc_contents");
    toc_ncx3.close();

    string content_opf = metadata.get("title_under") + "/OEPBS";
    string content_opf2 = content_opf + "/content.opf";
    ofstream content_opf3(content_opf2.c_str());
    content_opf3 << metadata.get("content_contents");
    content_opf3.close();

    string dest = content_opf + "/";
    for(int i=0;i<metadata.get_files_size();i++){
        copy_file(metadata.get_file(i), dest);
    }
    for(int i=0;i<metadata.get_images_size();i++){
        copy_file(metadata.get_image(i), dest);
    }

    string container_xml = metadata.get("title_under") + "/META-INF";
    string container_xml2 = container_xml + "/container.xml";
    ofstream container_xml3(container_xml2.c_str());
    container_xml3 << metadata.get("container_contents");
    container_xml3.close();

    string zip_mimetype = "zip " + 
                          metadata.get("title_under") + ".epub -DX0 " +
                          metadata.get("title_under") + "/mimetype";
    string zip_else = "zip " + 
                      metadata.get("title_under") + ".epub -rDX9 " + 
                      metadata.get("title_under") + "/META-INF " + 
                      metadata.get("title_under") + "/OEPBS";
//  cout << zip_mimetype << " and \n" << zip_else << endl;
    system(zip_mimetype.c_str());
    system(zip_else.c_str());
}
