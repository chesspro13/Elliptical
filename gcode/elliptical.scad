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
