

distX = 28.33+5;
distY = 45.75+5;
difference()
{
    cube([42,61,3], center=true);

    translate([distX/2,distY/2,-2])
    cylinder(4, 2.69,2.69);

    translate([distX/2,-distY/2,-2])
    cylinder(4, 2.69,2.69);

    translate([-distX/2,distY/2,-2])
    cylinder(4, 2.69,2.69);

    translate([-distX/2,-distY/2,-2])
    cylinder(4, 2.69,2.69);
}

height = 46.25;
back = 18;

$fn=30;

// Motor 
translate([-back,0,height])
{
    /*
    cube([39,42.25,42.25], center = true);
    translate([39/2,0,0])
    rotate([0,90,0])
    cylinder(24, 2.25, 2.25);*/
}


// Rotator
/*
translate([39/2+24-7-back,0,height])
rotate([0,90,0])
{
    difference()
    {
        translate([0,0,-7])
        cylinder(14,12.5, 12.5);

        // Slits
        translate([10,0,0])
        cube([10,30, 5.5], center=true);

        translate([-10,0,0])
        cube([10,30, 5.5], center=true);
        
        
        // Shaft
        translate([0,0,-8])
        cylinder(16, 5.25/2,5.25/2);
        $fn=30;
        
        // Holder thing
        sideX1 = 7;
        sideY1 = 5;
        translate([-sideX1,sideY1,-8])
        cylinder(16, 3,3);
        
        translate([sideX1,-sideY1,-8])
        cylinder(16, 3,3);
        
        sideX2 = 4.35;
        sideY2 = 7;
        // Holder thing cube
        translate([-sideX2,sideY2,-1])
        rotate([0,0,45])
        cube([6,5,150],center=true);
        
        translate([sideX2,-sideY2,-1])
        rotate([0,0,45])
        cube([6,5,50],center=true);
        
        
        
        // Holder thing 2
        sideX3 = 3;
        sideY3 = 9;
        translate([-sideX3,sideY3,-8])
        cylinder(16, 3,3);
        
        translate([sideX3,-sideY3,-8])
        cylinder(16, 3,3);
        
        
        // Holder thing 3
        sideX4 = 3;
        sideY4 = 11;
        translate([-sideX4,sideY4,-8])
        cylinder(16, 3,3);
        
        translate([sideX4,-sideY4,-8])
        cylinder(16, 3,3);
    }

    // Key Hole
    translate([-2,2,-7])

    cube([4,1,14]);
}*/

dist=30;

// Faceplate
translate([2.25,0,height])
rotate([0,90,0])
{
    difference()
    {
        // Plate
        cube([43,52,3], center = true);
        
        // Screw Holes
        translate([dist/2,dist/2,-2])
        cylinder(5, 1.75,1.75);
        
        translate([dist/2,-dist/2,-2])
        cylinder(5, 1.75,1.75);
        
        translate([-dist/2,dist/2,-2])
        cylinder(5, 1.75,1.75);
        
        translate([-dist/2,-dist/2,-2])
        cylinder(5, 1.75,1.75);
        
        translate([0,0,-2])
        cylinder(5, 27/2,27/2);
    }
}

// Block
difference()
{
    translate([-10,0,13])
    cube([55,40,24], center=true);
    
    translate([20,0,14])
    rotate([0,60,0])
    cube([34,48,16], center=true);
    
    translate([-40,0,14])
    rotate([0,60,0])
    cube([40,48,16], center=true);
    /*
    translate([-10,0,14])
    rotate([0,5,0])
    cube([15,48,16], center=true);
    
    translate([-10,0,13])
    rotate([5,0,90])
    cube([30,48,16], center=true);*/
}

difference()
{
    translate([-13,0,40])
    rotate([0,45,0])
    {
        translate([0,24,0])
        cube([15,3,60], center=true);

        translate([0,-24,0])
        cube([15,3,60], center=true);
    }

    // Face plate plate
    translate([9,0,55])
    cube([12,70,30], center=true);
}

translate([-35,0,20])
cube([10,50,10], center=true);