#include "mime.h"
#include <iostream>
#include <sstream>
#include <zim/writer/articlesource.h>
#include <zim/writer/zimcreator.h>
#include <zim/blob.h>
#include <cxxtools/log.h>

const char *svg_img_data =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
    "<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n"
    "    width=\"128px\" height=\"128px\" viewBox=\"0 0 12 13.836\" enable-background=\"new 0 0 12 13.836\" xml:space=\"preserve\">\n"
    "<path fill=\"#777777\" d=\"M1.938,6.656c-1.32,1.485-1.47,3.15-0.97,4.25c0.323,0.707,0.78,1.127,1.313,1.375\n"
    "    c0.496,0.229,1.074,0.273,1.658,0.282c0.023,0,0.04,0.03,0.062,0.03h4.187c0.61,0,1.225-0.125,1.75-0.405\n"
    "    c0.527-0.28,0.961-0.718,1.188-1.376c0.335-0.964,0.175-2.529-1.094-4.03C9.094,7.954,7.68,8.719,6.065,8.719\n"
    "    c-1.677,0-3.182-0.812-4.125-2.063H1.938z\"/>\n"
    "<path fill=\"#777777\" d=\"M6.063,0c-1.89,0-3.595,1.674-3.594,3.563C2.467,5.45,4.173,7.155,6.06,7.155\n"
    "    c1.89,0,3.564-1.705,3.563-3.593C9.625,1.673,7.95,0,6.063,0L6.063,0z\"/>\n"
    "</svg>\n";

struct A
{
    char ns;
    const char* url;
    const char* title;
    const char* mime;  // 0 for redirect
    const char* content;  // or redirect target
} a[] = {
  // article [0]
  { 'A', "0index.html", "0index", "text/html",
    "<html><body>\n"
    "<head><title>111-title</title><meta charset=\"UTF-8\"></head>\n"
    "<h1>Index Page</h1>\n"
    "<p><a href=\"../A/11111.html\">11111</a></p>\n"
    "<p><a href=\"../A/22222.txt\">22222</a></p>\n"
    "<p><a href=\"../A/33333.html\">33333</a></p>\n"
    "</body></html>\n" },
  // article [1]
  { 'A', "11111.html", "11111", "text/html",
    "<html><body>\n"
    "<head><title>111-title</title><meta charset=\"UTF-8\"></head>\n"
    "<link rel=\"stylesheet\" type=\"text/css\" href=\"../-/s/style.css\" />\n"
    "<h1>head1</h1><p>上海浦东国际机场</p>\n"
    "<img src=\"../I/s/user.svg\" alt=\"usr_img\">"
    "<h2>head2</h2><p><a href=\"../A/22222.html\">22222</a></p>\n"
    "<script src=\"../-/j/script.js\"></script>\n"
    "<input type=\"button\" onclick=\"display_alert()\" value=\"Alert\" />\n"
    "</body></html>\n" },
  // article [2]
  { 'A', "22222.txt", "22222", "text/plain", "article-22222" },
  // article [3]
  { 'A', "33333.html", "33333", 0, "1" },
  // resource(s)
  { '-', "s/style.css", "s/style.css", "text/css", "h1 {color:red; }" },
  { 'I', "s/user.svg", "s/user.svg", "image/svg+xml", svg_img_data },
  { '-', "j/script.js", "j/script.js", "application/x-javascript",
    "function display_alert()\n{\n\talert('hello');\n}\n" },
  // eof
  { '\0', 0, 0, 0 }
};

/****************************************************************/

class Article : public zim::writer::Article
{
    unsigned n;

public:
    explicit Article(unsigned n_ = 0)
      : n(n_)
      { }

    virtual std::string getAid() const;
    virtual char getNamespace() const;
    virtual std::string getUrl() const;
    virtual std::string getTitle() const;
    virtual bool isRedirect() const;
    virtual std::string getMimeType() const;
    virtual std::string getRedirectAid() const;
};

std::string Article::getAid() const
{
    std::ostringstream s;
    s << n;
    return s.str();
}

char Article::getNamespace() const
{
    return a[n].ns;
}

std::string Article::getUrl() const
{
    return a[n].url;
}

std::string Article::getTitle() const
{
    return a[n].title;
}

bool Article::isRedirect() const
{
    return a[n].mime == 0;
}

std::string Article::getMimeType() const
{
    return a[n].mime;
}

std::string Article::getRedirectAid() const
{
    return a[n].content;
}

/****************************************************************/

class ArticleSource : public zim::writer::ArticleSource
{
    Article article;
    unsigned n;

public:
    ArticleSource()
      : n(0)
      { }

    virtual const Article* getNextArticle();
    virtual zim::Blob getData(const std::string& aid);
};

const Article* ArticleSource::getNextArticle()
{
    if (a[n].ns)
    {
        article = Article(n++);
        return &article;
    }
    return 0;
}

zim::Blob ArticleSource::getData(const std::string& aid)
{
    std::istringstream s(aid);
    unsigned n;
    s >> n;
    const char* d = a[n].content;
    return zim::Blob(d, strlen(d));
}

/****************************************************************/

int main(int argc, char* argv[])
{
    try
    {
        log_init();
        zim::writer::ZimCreator creator(argc, argv);
        ArticleSource source;
        creator.create("foo-t.zim", source);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
