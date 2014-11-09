// TODO:
//   $Next should behave specially after chapter and paragraph breaks
//   Each chapter Hannah takes a break from working it
//   Reference the passing of time?
//   Variations on the core 'move' message?
                                           
#define STB_DEFINE
#include "stb.h"

#define NUM_DISKS   12


int replace(char **src, char *a, char *b)
{
   char *result;
   if (!strstr(*src, a))
      return 0;
   result = stb_dupreplace(*src, a, b);
   free(*src);
   *src = result;
   return 1;
}

char *choose3(char *a, char *b, char *c)
{
   int r = (rand() >> 3) % 3;
   if (r == 0) return a;
   if (r == 1) return b;
   return c;
}

char *choose4(char *a, char *b, char *c, char *d)
{
   int r = (rand() >> 7) % 4;
   if (r == 0) return a;
   if (r == 1) return b;
   if (r == 2) return c;
   return d;
}

#define sub(v,a)        replace(&body, v, a)
#define sub2(v,a,b)     replace(&body, v, rand()&64 ? a : b)
#define sub3(v,a,b,c)   replace(&body, v, choose3(a,b,c))
#define sub4(v,a,b,c,d) replace(&body, v, choose4(a,b,c,d))

int disk_print_count[NUM_DISKS];
int disk_last_printed[NUM_DISKS];

#define DISK_CHAPTER 4
#define DISK_PARAGRAPH 9

int action_number=1;
int chapter_start=2;
int paragraph_start=2;
int chapter_number = 1;

int first_seen = -1;
int been_a_while = -1;

void print_with_substitution(char *text)
{
   char *body = strdup(text);
   char *s;

   sub("$peg_0", "left peg");
   sub("$peg_1", "middle peg");
   sub("$peg_2", "right peg");

   // we have special rules for printing $disk_n, but there can only be one disk_n per string
   s = strstr(body, "$disk_");
   if (s) {
      char disk[16], buffer[256];
      int n;
      sscanf(s, "$disk_%d", &n);
      sprintf(disk, "$disk_%d", n);
      if (disk_last_printed[n] == 0) {
         // the first time we mention the disk, we give its full name -- this is when listing at the beginning
         if (n == 0)
            sprintf(buffer, "$nth_%d and largest disk, the $color_%d one,", n, n);
         else if (n == NUM_DISKS-1)
            sprintf(buffer, "$nth_%d and smallest disk, the $color_%d one,", n, n);
         else
            sprintf(buffer, "$nth_%d, $color_%d, disk", n, n);
      } else if (disk_last_printed[n] < chapter_start && n < NUM_DISKS-2) {
         // first time it's mentioned in this chapter, and it's not constantly mentioned
         sprintf(buffer, "$nth_%d ($color_%d) disk", n, n);
      } else if (disk_last_printed[n] < paragraph_start && n < NUM_DISKS-1) {
         // first time it's mentioned in this paragraph
         if (n <= DISK_PARAGRAPH)
            sprintf(buffer, "$nth_%d disk", n);
         else
            sprintf(buffer, "$nth_%d ($color_%d) disk", n, n);
      } else if (disk_last_printed[n] < chapter_start) {
         // first time it's mention in this chapter, 
         sprintf(buffer, "$nth_%d, $color_%d, disk", n, n);
      } else if (n < NUM_DISKS-1) {
         sprintf(buffer, "$nth_%d disk", n);
      } else
         sprintf(buffer, "$color_%d disk", n);
      sub(disk, buffer);


      // flag if we haven't seen this disk in a while
      if (disk_last_printed[n] < action_number-1000)
         if (disk_last_printed[n] == 0)
            first_seen = n;
         else {
            if (rand() % 100 < 150 - n * 20)
               been_a_while = n;
         }
      ++disk_print_count[n];
      disk_last_printed[n] = action_number;
   }

   // TODO: use different rule for chapter, paragraph
   sub4("$Next", "Next", "Then", "At this point", "Now");

   sub("$nth_10", "eleventh");
   sub("$nth_11", "twelfth");
   sub("$nth_12", "thirteenth");
   sub("$nth_13", "fourteenth");
   sub("$nth_0",  "first");
   sub("$nth_1",  "second");
   sub("$nth_2",  "third");
   sub("$nth_3",  "fourth");
   sub("$nth_4",  "fifth");
   sub("$nth_5",  "sixth");
   sub("$nth_6",  "seventh");
   sub("$nth_7",  "eighth");
   sub("$nth_8",  "ninth");
   sub("$nth_9",  "tenth");

   sub("$number_11", "eleven");
   sub("$number_12", "twelve");
   sub("$number_13", "thirteen");

   sub("$Number_11", "Eleven");
   sub("$Number_12", "Twelve");
   sub("$Number_13", "Thirteen");

   sub("$color_10", "maroon");
   sub("$color_11", "white");
   sub("$color_12", "peach");
   sub("$color_13", "black");
   sub("$color_0", "red");
   sub("$color_1", "orange");
   sub("$color_2", "brown");
   sub("$color_3", "yellow");
   sub("$color_4", "chartreuse");
   sub("$color_5", "green");
   sub("$color_6", "teal");
   sub("$color_7", "blue");
   sub("$color_8", "purple");
   sub("$color_9", "magenta");

   sub3("$before",       "before", "previously", "ever until then");
   sub("$noteworthy_0",  "monumental");
   sub3("$noteworthy_1", "major", "huge", "memorable");
   sub3("$noteworthy_2", "significant", "noteworthy", "relatively important");
   sub3("$event",        "event", "occasion", "occurrence");

   printf("%s", body);
   free(body);
}

void printsub(char *fmt, ...)
{
   char buffer[512];
   va_list v;
   va_start(v, fmt);
   vsprintf(buffer, fmt, v);
   va_end(v);
   print_with_substitution(buffer);
}

int first=1;
int trigger_chapter;
int pending=0;

// we just printed about this disk
void write_about_disk(int at_new_paragraph)
{
   int n;
   int first = (first_seen >= 0);
   n = first ? first_seen : been_a_while;
   assert(n >= 0);

   if (at_new_paragraph) {
      printsub("\n\n<p>\n\nThe $color_%d disk. ", n);
   } else {
      
   }
   if (first)
      printsub("Hannah hadn't moved that $before. ");
   if (n < 3) {
      printsub("A $noteworthy_%d $event. ");
   }

   printsub("\n<p>\n"
            "Hannah turned her attention back to solving the puzzle. ");

   first_seen = -1;
   been_a_while = -1;      
}

void move(int *pegs, int a, int b)
{
   int last_a = -1, last_b = -1;
   int i, disk, from, to;
   // find topmost disk on each peg
   for (i=0; i < NUM_DISKS; ++i) {
      if (pegs[i] == a)
         last_a = i;
      if (pegs[i] == b)
         last_b = i;
   }
   if (last_a < last_b) {
      disk = last_b;
      from = b;
      to = a;
   } else {
      disk = last_a;
      from = a;
      to = b;
   }
   pegs[disk] = to;
   if (first)
      printsub("First, ");
   else
      printsub("$Next ");
   first=0;

   printsub("Hannah moved the $disk_%d from the $peg_%d to the $peg_%d.\n", disk, from, to);

   // probably this never happens, but to be on the safe side, never print both events
   if (been_a_while && first_seen)
      been_a_while = -1;

   // decide whether to comment on this disk now or in the next paragraph
   // @TODO: real decision depends on what kind of comment you're going to make
   if (been_a_while >= 0 || first_seen >= 0) {
      if (!pending) {
         if (!first_seen) {
            if ((rand() & 256) < 80)
               pending = 1;
         }
         if (!pending)
            write_about_disk(0);
      }
   }      

   ++action_number;
   if (disk == DISK_PARAGRAPH) {
      printsub("\n<p>\n");
      paragraph_start = action_number;
      if (pending) {
         write_about_disk(1);
         pending = 0;
      }
      if (trigger_chapter) {
         printsub("\n\n<h2>Chapter %d</h2>\n\n", ++chapter_number);
         chapter_start = action_number;
         trigger_chapter = 0;
      }
   }
   if (disk == DISK_CHAPTER) {
      trigger_chapter=1;
   }
}

int check_done(int *pegs)
{
   int i;
   for (i=0; i < NUM_DISKS; ++i)
      if (pegs[i] != 2)
         return 0;
   return 1;
}

void hanoi(void)
{
   int pegs[NUM_DISKS] = { 0 };
   int n = 0;
   for(;;) {
      move(pegs, 0, 1); if (check_done(pegs)) return;
      move(pegs, 0, 2); if (check_done(pegs)) return;
      move(pegs, 1, 2); if (check_done(pegs)) return;
   }
}


int main(int argc, char **argv)
{
   int i;
   srand(0);
   printsub("<h1>How Hannah Solved The $Number_%d-Disk Tower of Hanoi</h1>\n\n", NUM_DISKS);
   printsub("\n\n<h2>Prologue</h2>\n\n");
   printsub("Hannah was faced with a puzzle known as the Tower of Hanoi.<p> The puzzle consisted of three tall pegs side-by-side,\n"
            "and a stack of $number_%d disks threaded through the left peg.\n<p>\n", NUM_DISKS);
   printsub("The $number_%d disks were successively smaller from bottom to top, and each was colored differently.\n", NUM_DISKS);
   printsub("The first, bottom-most, and largest disk was colored $color_0;\n");
   for (i=1; i < NUM_DISKS-1; ++i) {
      if (i > 1 && i < NUM_DISKS-2 && (rand() & 127) < 50)
         printsub("the next, $nth_%d, disk was colored $color_%d;\n", i, i);
      else
         printsub("the next disk was colored $color_%d;\n", i);
   }
   printsub("and the $nth_%d, smallest, and top-most disk was colored $color_%d.\n<p>\n", i, i);
   printsub("What Hannah needed to do to solve the puzzle was to move one disk at a time from the top\n"
            "of a stack on one peg to another peg, never placing a disk on top of a smaller disk,\n"
            "so that all $number_%d disks ended up on the right peg.\n", NUM_DISKS);
   printsub("\n\n<p>\n\n");
   printsub("\n\n<h2>Chapter 1</h2>\n\n");
   hanoi();
   return 0;
}
