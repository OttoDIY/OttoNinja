// Part to print
part="all"; // [all:Preview Bot,head:Head,body:Body,leg:Leg,foot_left:Left Foot,foot_right:Right Foot]

// Brain type (hole pattern in head)
brain_type="nano_shield"; // [nano_shield:Stock (Arduino Nano + Servo Shield),nano_proto:Arduino Nano + Proto board,uno_shield:Arduino Uno + Servo Shield,uno:Arduino Uno (and wires)]

// Servo Type for hip
servo_type_hip="sg90"; // [sg90:SG90 (nylon gear micro servo),mg90s:MG90S (metal gear micro servo)]

// Servo Type for ankle
servo_type_ankle="sg90"; // [sg90:SG90 (nylon gear micro servo),mg90s:MG90S (metal gear micro servo)]

// Body Width (Head width is based off this)
body_width=67; // [67:200]

// Body Depth (Head depth is based off this)
body_depth=68; // [68:200]

// Body Height
body_height=38; // [20:200]

// Head Height
head_height=34; // [20:200]

// Body Shell Thickness
body_thickness=1.75; // [0.25:0.25:8]

// Head Shell Thickness
head_thickness=2; // [0.25:0.25:8]

// Nose Diameter (6 = Small Toggle Switch)
nose_diameter=6; // [0:24]
// Vertical distance between center of eyes and nose
nose_distance=15; // [-8:190] 

// Brain Screw Size (2 = Stock)
brain_screw_size=2; // [2:5]

// Make Brain Screw thru Skull
brain_screw_thru=0; // [1:Yes,0:No]

// Solid leg
leg_solid=0; // [1:Yes, solid,0:No, my printer doesn't like those overhangs]

/* [Hidden] */
part_clearance=0.25;

arduino_holes=[[-27,-7],[-27,20.9],[25.2,26.1],[23.825,-22.25]];
proto_holes=[[27,-10],[-17.9,-10],[26,25.5],[-17.7,25.5]];

print_part();

module print_part() {
  warnings()
    draw_part();
}

module draw_part() {
  if(part=="head")
    head();
  else if(part=="body")
    body();
  else if(part=="leg")
    leg();
  else if(part=="foot_left")
    foot();
  else if(part=="foot_right")
    mirror([1,0]) foot();
  else if(part=="all")
    rotate([0,0,-90]) otto();
}
module otto() {
  head(move=1);
  body(move=1);
  mirrory() {
    translate([0,body_width/2-33.5,0])
    {
      mirror([1,0]) foot(move=1);
      translate([0,24,-1]) mirror([0,0,1]) leg(move=0);
    }
  }
}
function brain_type_text() = (
  brain_type=="nano_shield"?"Arduino Nano + Servo Shield":(
  brain_type=="uno_shield"?"Arduino Uno + Servo Shield":(
  brain_type=="nano_proto"?"Arduino Nano + Proto board":"Unknown")));
module warnings() {
  if(brain_type=="nano_shield")
  {
    if(body_width<67)
      error(str("If ",brain_type_text()," is used, Body Width must be >= 67!"));
    else if(body_depth<67)
      error(str("If ",brain_type_text()," is used, Body Depth must be >= 67!"));
    else children();
  } else if(brain_type=="uno_shield"||brain_type=="uno")
  {
    if(body_width<81)
      error("If Arduino Uno is used, body width MUST be greater than 81!");
    else children();
  } else children();
}
module error(s) {
  echo(str("***ERROR*** ",s));
  rotate([0,0,-45])
        rotate([80,0,0]) color("red") text(s);
}
module body(dims=[body_depth-1,body_width,body_height]) {
  left=dims[0]/-2; // -33.5
  botm=dims[1]/-2; // -33.5
  right=dims[0]/2; // 34
  servozoff=servo_type_hip=="sg90"?2:0;
  *#import("Otto_DIY/files/OTTO_body_v3.stl");
  mirrorx(0.4) mirrory(0.25) translate([left+body_thickness,botm+body_thickness+13.775,body_thickness]) {
      cube([dims[0]/2-9,2.5,12.4]);
      translate([0,2.5,12.39]) rotate([90,0,90]) mirror([1,0]) linear_extrude(dims[0]/2-9,convexity=10) polygon([[0,0],[1.2,0.825],[2.5,0.825],[2.5,0]]);
  }
  mirrory() mirrorx() translate([left+body_thickness-.01,botm+6,dims[2]-16]) difference(){
    cube([2,13.7,5]);
    translate([0,-.01]) rotate([0,41]) cube([3,13.72,5]);
  }
  mirrory() for(x=[0,28.2]) translate([x-9.5,botm+body_thickness,body_thickness]) difference() {
    union(){
      cube([2,13.775,6.18-servozoff]);
      translate([0.98,6.85,0]) cylinder(d1=5.7,d2=4.4,h=0.85,$fn=30);
      translate([0.98,6.85,0.84]) cylinder(d=4.4,h=5.35-servozoff,$fn=40);
    }
      translate([0.98,6.85,-.01]) cylinder(d=1.9,h=7,$fn=20);
  }
  for(mx=[0,1],my=[0,1]) mirror([mx,0]) mirror([0,my]) translate([mx?-11:0,0])
  translate([left+body_thickness+22.2,botm+body_thickness,body_thickness]) difference() { union() {
      if(!mx&&!my)
        translate([-10.7,6.61])
          cylinder(d=16,h=6.25-servozoff,$fn=50);
      else if(!mx&&my)
        translate([-17.5,0,.01]) cube([11.9,13.2,6-servozoff]);
      translate([-1-(left+33),5.64,0]) mirror([1,0]) cube([(mx?-22:-12)-left,2,6.1-servozoff]);
    }
    if(!mx&&!my)
      translate([-10.72,6.61,-.01]) cylinder(d=12,h=6.27,$fn=40);
    else if(!mx&&my)
      translate([-15.9,5,-.01]) {
        cube([8.35,8.2,7]);
        translate([0,-6]) cube([8.35,4,7]);
      }
  }
  difference() {
      translate([left,botm,0]) box_chamfer(dims,ch=2,chvx=5,chvy=5);
//    translate([left+1.75,botm+1.75,2]) box_chamfer([dims[0]-3.5,dims[1]-3.5,dims[2]],ch=1,chvx=4,chvy=4,inside=1);
    translate([left+body_thickness,botm+body_thickness,2]) box_chamfer([dims[0]-body_thickness*2,dims[1]-body_thickness*2,dims[2]],ch=1,chvx=4,chvy=4,inside=1);
    translate([0,0,-1]) linear_extrude(10,convexity=10) {
      mirrory() {
        translate([.1,botm+8.5]) circle(d=12,$fn=30);
        translate([6,botm+8.5]) circle(d=servo_type_hip=="mg90s"?6.5:7,$fn=20);
        offset(-1) offset(1)
        translate([-14.5,-15]) square([29,4]);
      }
      translate([left+9,-botm-13.5]) square([5.2,5.2]);
      translate([left+12.8,botm+8.35]) {
        circle(d=2,$fn=20);
        for(r=[2:360/7:359]) rotate([0,0,r]) translate([4.1,0]) circle(d=2,$fn=20);
      }
    }
    mirrory() translate([-11.7,right-body_thickness-.51,dims[2]-11]) {  cube([22.35,body_thickness+0.6,4]);
      translate([0,body_thickness+.02,4.1]) rotate([-35,0]) mirror([0,0,1]) mirror([0,1,0]) cube([22.35,4,5.4]);
    }
  }
}
module head(dims=[
    body_width-(body_thickness+part_clearance)*2,
    body_depth-(body_thickness+part_clearance)*2,
    head_height],brain_type=brain_type,head_thickness=head_thickness,move=0) {
  left=dims[0]/-2; // -31.5
  botm=dims[1]/-2; // -32.25
  right=dims[0]/2; // 31.5
  translate(move?[0,0,body_height+head_height-3]:[]) rotate(move?[180,0,-90]:[]) {
    //#import("Otto_DIY/files/OTTO_head_v3.stl");
    difference(){
      translate([left,botm,0]) box_chamfer(dims,ch=2,chy1=3.5,chvx=5.5,chvy=4);
      translate([left+head_thickness,botm+head_thickness+1.501,head_thickness]) box_chamfer([dims[0]-head_thickness*2,dims[1]-head_thickness*2-1.5,dims[2]],ch=1,chvx=2.75,chvy=2.75,inside=1);
      translate([left,botm-.01,dims[2]-3.8]) cube([dims[0],1.5,10]);
      for(x=[-13.25,13.25])
        translate([x,botm-.01,15]) rotate([-90,0]) cylinder(d=16.5,h=4,$fn=50);
      brain_holes(brain_type,top=dims[1]/2-20,right=dims[0]/2);
      translate([-5.6,-29.555,3.75]) {
        translate([11.25,0]) rotate([-90,0]) rotate([0,0,180]) intersection() {
          box_chamfer([11.3,7.6,2],ch=0,chvx=1.6,chvy=1.6);
          translate([0,0,-.8]) box_chamfer([11.3,7.6,4],ch=1.6,chvx=0,chvy=0);
          
        }
      }
      if(nose_diameter>0&&dims[2]-(nose_diameter/2+nose_distance)>18)
      {
        translate([0,botm-.01,15+nose_distance]) rotate([-90,0]) cylinder(d=nose_diameter,h=head_thickness+2.02,$fn=30);
      } else echo(str("Warning: Hiding nose."));
    }
    // tabs
    mirrorx() translate([-left-2,-9.25,dims[2]]) rotate([-90,0]) mirror([0,1,0]) linear_extrude(21,convexity=10) polygon([[0,0],[0,8.4],[1.98,8.4],[4.1,6.3],[4.1,4.2],[2,2.5],[2,0],]);
    mirrory(0.75) mirrorx(0) translate([left+4.5,botm+1.5,dims[2]]) rotate([90,0]) mirror([0,0,1]) linear_extrude(2) polygon([[0,0],[0,6],[2,8],[12,8],[14,6],[14,1.5],[15.5,0]]);
    brain(brain_type=brain_type,top=dims[1]/2-20,right=dims[0]/2);
  }
}
module leg(move=0) {
  bo=servo_type_ankle=="sg90"?0:(
    servo_type_ankle=="mg90s"?1.6:0);
  translate(move?[0,24,-40]:[]) rotate([0,0,0]) union(){
  //#import("Otto_DIY/files/OTTO_leg_v4.stl");
  difference() {
    translate([-14.5,-8.25,0]) {
      intersection() {
        cube([29,16.5,38.5]);
        union(){
          box_chamfer([29,16.5,20],2);
          translate([0,0,38.6]) mirror([0,0,1]) box_chamfer([29,16.5,20],1.25,chvx=2,chvy=2);
          *translate([1,1,30])
            box_chamfer2([29-2,16.5-2,7],2);
        }
      }
    }
    translate([-12.5,-6.25,4.25]) cube([25,12.5,50]);
    translate([12,-3,5]) cube([6,6,7.5]);
    translate([0.5,0,2.6-(servo_type_hip=="mg90s"?0:1)]) rotate([0,0,90]) {
      linear_extrude(3.4) 
        polygon([[-3.1,0],[-2,20],[2,20],[2.8,0]]); 
      translate([0,0,-3.01]) cylinder(d=servo_type_ankle=="mg90s"?7.8:7.4,h=8,$fn=30);

    }
    translate([-14.5-.01,0,28.5]) rotate([0,90,0]) {  cylinder(d=4,h=5,$fn=30);
      rotate([0,0,90]) translate([0,0,5]) {
        cylinder(d=12,h=30,$fn=30);
        translate([0,-5.5]) cylinder(d=servo_type_ankle=="mg90s"?6.5:7,h=30,$fn=20);
        translate([-6,0]) cube([12,30,30]);
      }
    }
    if(!leg_solid)
      mirrory() translate([-11.525,-8.3,4.9]) mirror([0,1,0]) rotate([90,0]) linear_extrude(5) {
      //square([21.565,12.93]);
      polygon([[0,0],[0,5.7],[6.1,12.93],[21.565-5.7,12.93],[21.565,5.7],[21.565,0]]);
    }
    translate([-15,0,9]) rotate([0,90,0]) cylinder(d=2.5,h=5,$fn=20);
  }
  difference() {
    translate([8.55-bo,-3.8,4]) cube([4+bo,7.6,7.5]);
    translate([10-bo,-3,5]) cube([4+bo,6,8]);
    translate([8.5-bo,0,9]) rotate([0,90,0]) cylinder(d=2.2,h=2,$fn=20);
  }
}
}
module foot(move=0) {
  servohornoff=(servo_type_ankle=="sg90"?2:0);
  translate(move?[0,34.5,-48]:[])
  //*translate([0,44,0]) import("Otto_DIY/files/OTTO_footL_v4.stl");
  difference(){
    union(){
      translate([-31,-20,0]) {
        intersection() {
          cube([62,40,5]);
          translate([0,0,-3]) box_chamfer3([62,40,8.01],2);
          *translate([2.8,2.8,0]) box_chamfer2([56.3,34.4,2.5],4);
        }
      }
      translate([15.8,-9.5,15.75]) sphere(d=4,$fn=30);
      for(m=[0,1]) mirror([m,0])
      translate([-21.15,-20,0]) difference() {
        intersection(){
          cube([4+(m?1.1:1.5),21,24.5+(m?0:2)]);
          box_chamfer3([41.8,21,25+(m?0:2)],2);
          *translate([2.8,2.8,0]) box_chamfer2([36.2,15.4,22.5],4);
        }
        translate([5.5,-.01,-.01]) cube([31.3,21.02,25.02]);
        if(m==0)
        translate([-.01,10.5,15.5]) rotate([0,90]) {
            cylinder(d=7.3,h=6+servohornoff,$fn=30);
            translate([-8,0]) cylinder(d=1,h=6+servohornoff,$fn=10);
            rotate([0,0,90]) linear_extrude(2+servohornoff) {
            polygon([[-3.5,0],[-2,10+(m?0:2)],[2,10+(m?0:2)],[3.5,0]]);
          }
        }
      }
    }
    translate([-15.7,-21.01,1]) {
      translate([0,-1,0]) box_chamfer3([31.8,26.02,10],2);
      *intersection(){
        cube([31.8,25.02,10]);
        translate([2,0,2.8]) box_chamfer2([27.3,22,8],4);
      }
    }
  }
}
module brain_holes(brain_type=brain_type,
    right=(body_width-(body_thickness+part_clearance)*2)/2,
    top=(body_depth-(body_thickness+part_clearance)*2)/2-20) {
    blo=(brain_type=="uno_shield"||brain_type=="uno"?14:0);
    bto=(brain_type=="uno_shield"?4:(brain_type=="nano_shield"?-.5:0));
  if(brain_type=="nano_proto") {
    translate([right-2.76,-.2,20.5]) rotate([0,90]) rotate([0,0,90]) usb_mini();
  }
  if(brain_type=="nano_shield"||brain_type=="uno_shield"||brain_type=="uno")
  {
    translate([right-31.5-blo,top-12.5-bto,brain_screw_thru?-.01:head_thickness]) for(pos=arduino_holes) translate(pos) cylinder(d=brain_screw_size,h=3+head_thickness+.02,$fn=24);
  }
  if(brain_type=="nano_shield") {
    translate([0,top-12])
    {
    translate([right+.01,18.163,12.875]) rotate([0,-90]) cylinder(d=8,h=4,$fn=24);
    translate([right-2.76,12.5,4.815]) cube([2.06,10.5,14.2]);
    translate([right-2.76,-2.4,19.5]) rotate([0,90]) rotate([0,0,90]) usb_mini();
    }
  } else if(brain_type=="uno_shield"||brain_type=="uno") {
    translate([right-head_thickness-.01,top,7]) {
      cube([head_thickness+.02,8.9+part_clearance*2,10.9+part_clearance*2]);
      translate([0,-31.7]) cube([head_thickness+.02,12+part_clearance*2,10.9+part_clearance*2]);
    }
  }
}
module brain(brain_type=brain_type,
  right=(body_width-(body_thickness+part_clearance)*2)/2,
  top=(body_depth-(body_thickness+part_clearance)*2)/2-20)
{
  blo=(brain_type=="uno_shield"||brain_type=="uno"?14:0);
  bto=(brain_type=="uno_shield"?4:(brain_type=="nano_shield"?-.5:0));
  translate([right-31.5-blo,top-12.5-bto]) {
  if(brain_type=="nano_shield"||brain_type=="uno_shield"||brain_type=="uno") {
    translate([-25.4,-25,2]) mirror([0,1]) cube([2,4.9+(top-12),3.9]);
    translate([-27-brain_screw_size/2,-8,2]) mirror([1,0]) cube([3-brain_screw_size/2+(right-34),2,3]);
    translate([-27-brain_screw_size/2,20,2]) mirror([1,0]) cube([3-brain_screw_size/2+(right-34),1.98,3]);
    translate([-25.375,29.075,2]) cube([2,1.5+bto,3.9]);
    translate([23.626,29.075,2]) cube([2,1.5+bto,3.9]);
    translate([27,25,2]) cube([4,2,3]);
    translate([22.877,-24,2]) mirror([0,1]) {
      translate([0,1.125]) cube([2,4.875+(top/2+bto-6),3.9]);
      cube([2,6,3]);
    }
    translate([0,0,1.99]) for(pos=arduino_holes)
      translate(pos) brain_screw();
  } else if(brain_type=="nano_proto") {
    translate([0,2,2]) cube([10,4,3]);
    translate([0,0,2]) for(pos=proto_holes)
       translate(pos) {
         difference() {
           union() {
             brain_screw();
             if(pos[1]>0)
              translate([-1,2]) cube([2,4,3]);
             else if(pos[0]>15)
               translate([0,-1]) cube([4,2,3]);
           }
           translate([0,0,-.01]) cylinder(d=2,h=4.02,$fn=20);
       }
     }
     }
  }
}
module brain_screw(brain_screw_size=brain_screw_size) {
  difference() {
    union(){
      cylinder(d=brain_screw_size+2.3,h=3,$fn=30);
      cylinder(d1=brain_screw_size+4,d2=brain_screw_size+2.3,h=1,$fn=30);
    }
    translate([0,0,-.01]) cylinder(d=brain_screw_size,h=4.02,$fn=20);
  }
}
module usb_mini() {
  linear_extrude(3,convexity=10) {
    translate([0.6,0]) union() {
      translate([0,0.1]) square([8-1.202,4]);
      for(x=[0.1,6.7]) translate([x,0.1]) circle(r=0.1,$fn=20);
      translate([0.1,0]) square([6.6,4]);
    }
    mirrorx(3.999) translate([0.4,1.81]) difference() {
      square([0.2,0.2]);
      circle(r=0.2,$fn=20);
    }
    translate([0,2.008]) union() {
      translate([0.1,0.2]) square([7.8,1.9]);
      translate([0.2,0]) square([7.6,2.1]);
      translate([0,0.1]) square([8,1.9]);
      for(x=[0.2,7.8])
        translate([x,0.2]) circle(r=0.2,$fn=20);
      for(x=[0.1,7.9])
        translate([x,2]) circle(r=0.1,$fn=20);
    }
  }
}

module box_chamfer3(dims,ch=1,bot=1)
{
  render() intersection() {
    cube(dims);
    minkowski()
    {
      translate([ch,ch,ch]) cube([dims[0]-ch*2,dims[1]-ch*2,dims[2]-ch*2]);
      *sphere(r=ch/sin(60),$fn=10);
      for(m=[0,1]) mirror([0,0,m]) 
        cylinder(r1=ch,r2=m?(bot?0:ch):0,h=ch,$fn=4);
    }
  }
}
module box_chamfer2(dims,ch=1)
{
  render() minkowski()
  {
    cube(dims);
    intersection()
    {
      rotate([45,0,45]) cube(ch,center=true);
      rotate([45,0,-45]) cube(ch,center=true);
    }
  }
}
module box_chamfer(dims,ch=1,chx1="auto",chx2="auto",chy1="auto",chy2="auto",chvx="auto",chvy="auto",inside=0) {
  x1=chx1=="auto"?ch:chx1;
  x2=chx2=="auto"?ch:chx2;
  y1=chy1=="auto"?ch:chy1;
  y2=chy2=="auto"?ch:chy2;
  vx=chvx=="auto"?ch:chvx;
  vy=chvy=="auto"?ch:chvy;
  difference(){
    cube(dims);
    if(x1)
    {
      translate([x1,-.01]) rotate([0,-45]) mirror([1,0]) cube([x1*2,dims[1]+.02,x1*2]);
      *if(vx)
      translate([9.9,0]) rotate([0,0,45]) translate([-5+x1,0]) translate([x1,-.01]) rotate([0,-45]) mirror([1,0])  cube([x1*2,dims[1]+.02,x1*2]);
    }
    if(x2)
      translate([dims[0]-x2,-.01]) rotate([0,45]) cube([x2*2,dims[1]+.02,x2*2]);
    if(y1)
      translate([-.01,y1]) rotate([45,0]) mirror([0,1,0]) cube([dims[0]+.02,y1*2,y1*2]);
    if(y2)
      translate([-.01,dims[1]-y2]) rotate([-45,0]) cube([dims[0]+.02,y2*2,y2*2]);
    if(vx)
    {
      mirrorx(dims[0]/2) {
        translate([vx,0,-.01]) rotate([0,0,45]) mirror([1,0]) cube([vx*2,vx*2,dims[2]+.02]);
        translate([inside?-1.4:-1.5,inside?0:-1.1]) rotate([0,0,45])
        mirror([1,0,0]) rotate([0,45,0])
        translate([-vx,-vx]) translate([0,0,vx/-.75]) cube([10,vx*2,vx*2]);
      }
    }
    if(vy)
      mirrorx(dims[0]/2) {
        translate([0,dims[1]-vy,-.01]) rotate([0,0,-45]) mirror([1,0]) cube([vy*2,vy*2,dims[2]+.02]);
        translate([0,dims[1]+(inside?-0.25:0.5),-.01])
          rotate([0,0,-45]) mirror([1,0,0])
          rotate([0,-45,0])
          translate([-vx,-vx]) translate([0,0,vx/-.75]) 
          cube([10,vx*2,vx*2]);
      }
  }
}
module mirrory(offset=0)
{
  translate([0,offset,0]) for(m=[0,1]) mirror([0,m,0]) translate([0,offset*-1,0]) children();
}
module mirrorx(offset=0)
{
  translate([offset,0]) for(m=[0,1]) mirror([m,0]) translate([offset*-1,0]) children();
}
module otto_stl() {
  translate([0,0,66]) rotate([180,0,-90]) import("Otto_DIY/files/OTTO_head_v3.stl");
  import("Otto_DIY/files/OTTO_body_v3.stl");
  translate([0,-34.5,-48]) import("Otto_DIY/files/OTTO_footR_v4.stl");
  translate([0,34.5,-48]) import("Otto_DIY/files/OTTO_footL_v4.stl");
  for(y=[-25,25])
    translate([0,y,-40]) rotate([0,0,0]) import("Otto_DIY/files/OTTO_leg_v4.stl");
}