#include <iostream>
#include <sstream>
#include <zim/writer/articlesource.h>
#include <zim/writer/zimcreator.h>
#include <zim/blob.h>
#include <cxxtools/log.h>

struct A
{
    char ns;
    const char* url;
    const char* title;
    const char* mime;  // 0 for redirect
    const char* content;  // or redirect target
} a[] = {
  // article
  { 'A', "11111.html", "11111", "text/html",
    "<html><body>"
    "<head><title>111-title</title><meta charset=\"UTF-8\"></head>"
    "<link rel=\"stylesheet\" type=\"text/css\" href=\"../-/s/style.css\" />"
    "<h1>head1</h1>三清山"
    "<h2>head2</h2>上海浦东国际机场"
    "</body></html>" },
  { 'A', "22222.html", "22222", "text/plain", "Auto22222" },
  { 'A', "33333.html", "33333", "text/plain", "Auto33333" },
  // resource
  { '-', "s/style.css", "s/style.css", "text/plain", "h1 {color:red; }" },
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
