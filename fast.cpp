#define _CRT_SECURE_NO_WARNINGS
#ifdef TIMER
#include <boost\timer\timer.hpp>
#endif
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#define CALLOC(T,N) (T*)calloc(N,sizeof(T))

#define MY_CHAR uint_fast8_t
#define MY_INT int_fast32_t
const MY_INT BUF = 1 << 14;

MY_CHAR i_buf[BUF + 1];

MY_INT bigArr[250000000] = {};

class fi
{
public:
    fi(const char*& fileName) :pos(nullptr), cnt(0), end(nullptr){
        fp = fopen(fileName, "r");
    }

    MY_INT gi()
    {
        MY_INT i = 0;
        MY_CHAR ch = gc();
        while (ch == '\t' || ch == '\n')
        {
            ch = gc();
        }
        while (ch >= '0' && ch <= '9')
        {
            i = (i * 10) + ch - '0';
            ch = gc();
        }
        return i;
    }

    MY_CHAR gc()
    {
        if (pos == end)
        {
            cnt = fread(i_buf, 1, BUF, fp);
            if (!cnt)
                return 0;
            end = i_buf + cnt;
            pos = i_buf;
        }
        return *pos++;
    }
private:
    FILE* fp;
    MY_CHAR* pos;
    MY_INT cnt;
    MY_CHAR* end;
};

class fo
{
    MY_CHAR o_buffer[BUFSIZ];
    MY_CHAR* bufloc;
    FILE* fp;
    MY_INT pos;
public:
    fo(const char*& fileName) :pos(0){ fp = fopen(fileName, "w"); }
    ~fo(){ fwrite(o_buffer, 1, pos, fp); fflush(fp); fclose(fp); }

    void write(MY_CHAR* fmt, int val)
    {
        if (pos >= BUFSIZ - 20)
        {
            fwrite(o_buffer, 1, pos, fp);
            pos = 0;
        }
        MY_CHAR* loc = fmt;
        while (*loc)
        {
            o_buffer[pos++] = *loc++;
        }
        o_buffer[pos++] = ' ';
        o_buffer[pos++] = ':';
        o_buffer[pos++] = ' ';

        MY_INT offset = val < 10000 ? (val < 100 ? (val < 10 ? 1 : 2) : (val < 1000 ? 3 : 4)) :
            (val < 10000000 ? (val < 1000000 ? (val < 100000 ? 5 : 6) : 7) : val < 100000000 ? 8 : 9);
        WriteInt(val, offset);
        pos += offset;
        o_buffer[pos++] = '\n';
    }
private:
    void WriteInt(MY_INT val, MY_INT offset)
    {
        while (offset)
        {
            o_buffer[pos + offset - 1] = val - (val / 10 * 10) + '0';
            val /= 10;
            offset--;
        }
    }
};

class ReadWrite
{
public:
    void Query(const char*& queries, const char*& output)
    {
        fi qr(queries);
        fo fileOut(output);
        MY_CHAR c, loc, out[10], *op;
        do
        {
            c = qr.gc();
            while (c == '\t' || c == '\n')
                c = qr.gc();
            if (!c)
                return;
            op = out;
            bool isValid = true;
            int i = 0;
            while (c >= '0' && c <= '9')
            {
                *op++ = c;
                i = (i * 10) + c - '0';
                c = qr.gc();
            }
            if (*(bigArr+i))
            {
                *op = 0;
                fileOut.write(out, *(bigArr + i));
            }
        } while (c);
    }

    void Read(const char*& inp)
    {
        fi fr(inp);
        MY_INT i;
        do
        {
            i = fr.gi();
            (*(bigArr+i))++;
        } while (i);
    }
};

int main()
{
#ifdef TIMER
    boost::timer::auto_cpu_timer t;
#endif
    const char* fileName = "C:/pratik/ThirdParty/GraphFile/ungraph.txt";
    const char* queries = "C:/pratik/ThirdParty/GraphFile/queries";
    const char* output = "C:/pratik/ThirdParty/GraphFile/out.txt";
    ReadWrite f;
    f.Read(fileName);
    f.Query(queries,output);
    return 0;
}
