#include"OS.H" 
#include"OS_WGH.H"
#include"OS_LJF.H"
#include"OS_JWT.H"
#include"OS_WLM.H"

int main()
{
    int choice;
    while(choice=menu())
    {
       switch(choice)
       {
          case 1:wgh_show_time();break;
		  case 2:jwt_show_time();break;
		  case 3:ljf_show_time();break;
		  case 4:wlm_show_time_sync();break;
		  case 5:wlm_show_time_buffer();break; 
		  case 6:
		  default:return 0; 
       }
    }
}


