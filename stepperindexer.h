#include <StepperIndexer.h>




//*********************************************
//        Indexeur pour Moteur Pas à Pas
//*********************************************
//  version: V1.0
// Auteur: Christian TROUILLET

int32_t cptmes2;
int32_t cptmes3;

 TStepperIndexer MyStepper(2,3);
 
void SetupStepperIndexer()
{
StepperSetupHardWare();
MyStepper.SetAccel(memory.d.mot_accel);
}


#define print_Mot_1Tr
void Mot_1Tr(void)
{
int new_pos;  
if(!MyStepper.SpeedZero)
return;

//GoToTarget(int32_t Target,uint16_t Speed)

if (MyStepper.Position !=0)
  new_pos = 0;// retour position 0
else
  new_pos = memory.d.mot_pas;// avance d'un tour

MyStepper.SetAccel(memory.d.mot_accel);
MyStepper.GoToTarget(new_pos,memory.d.mot_vit);

#ifdef print_Mot_1Tr
Serial.print("MyStepper Position ");  
Serial.println(MyStepper.Position);  
Serial.print("MyStepper.GoToTarget: step: ");  
Serial.print(new_pos);  
Serial.print(" vit: ");  
Serial.println(memory.d.mot_vit);
Serial.print(" accel: ");  
Serial.println(memory.d.mot_accel);
#endif
}
