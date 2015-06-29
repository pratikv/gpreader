#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#define CALLOC(T,N) (T*)calloc(N,sizeof(T))

#define MY_CHAR uint_fast8_t
#define MY_INT int_fast32_t
const MY_INT BUF = 1 << 14;

MY_CHAR i_buf[BUF + 1];

struct Node
{
    Node* c[10];
    MY_INT cnt;
};

void CleanUp(Node** node)
{
    if (*node)
    {
        for (int i = 0; i < 10; i++)
        {
            CleanUp(&((*node)->c[i]));
        }
        free(*node);
        *node = nullptr;
    }
}

class fi
{
public:
    fi(const char*& fileName) :pos(nullptr), cnt(0), end(nullptr){
        fp = fopen(fileName, "r");
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
    Node* head;
public:
    ReadWrite() :head(CALLOC(Node, 1))
    {
    }

    ~ReadWrite()
    {
        CleanUp(&head);
    }

    void Query(const char*& queries, const char*& outFile)
    {
        fi qr(queries);
        fo output(outFile);
        MY_CHAR c, loc, out[10], *op;
        do
        {
            c = qr.gc();
            while (c == '\t' || c == '\n')
                c = qr.gc();
            if (!c)
                return;
            Node* ptr = head;
            op = out;
            bool isValid = true;
            int cnt = 0;
            while (c >= '0' && c <= '9')
            {
                if (isValid)
                {
                    *op++ = c;
                    if (ptr->c[loc = c - '0'] == nullptr)
                        isValid = false;
                    ptr = ptr->c[loc];
                }
                c = qr.gc();
            }
            if (isValid && ptr->cnt)
            {
                *op = 0;
                output.write(out, ptr->cnt);
            }
        } while (c);
    }

    void Read(const char*& inp)
    {
        fi fr(inp);
        MY_CHAR c;
        MY_CHAR loc;
        do
        {
            c = fr.gc();
            while (c == '\t' || c == '\n')
                c = fr.gc();
            Node* ptr = head;
            while (c >= '0' && c <= '9')
            {
                if (ptr->c[loc = c - '0'] == nullptr)
                {
                    ptr->c[loc] = CALLOC(Node, 1);
                }
                ptr = ptr->c[loc];
                c = fr.gc();
            }
            ptr->cnt++;
        } while (c);
    }
};

int main()
{
    const char* fileName = "C:/pratik/ThirdParty/GraphFile/ungraph.txt";
    const char* queries = "C:/pratik/ThirdParty/GraphFile/queries";
    const char* output = "C:/pratik/ThirdParty/GraphFile/out.txt";
    ReadWrite f;
    f.Read(fileName);
    f.Query(queries, output);
    return 0;
}
