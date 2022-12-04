; ModuleID = '.kernel_3mm.cpp.ll'
source_filename = "src/kernel_3mm.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10kernel_3mmPA10_iS0_S0_S0_S0_S0_S0_([10 x i32]* %A, [10 x i32]* %B, [10 x i32]* %C, [10 x i32]* %D, [10 x i32]* %E, [10 x i32]* %F, [10 x i32]* %G) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc24, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc25, %for.inc24 ]
  %cmp = icmp slt i32 %i.0, 10
  br i1 %cmp, label %for.body, label %for.end26

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc21, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc22, %for.inc21 ]
  %cmp2 = icmp slt i32 %j.0, 10
  br i1 %cmp2, label %for.body3, label %for.end23

for.body3:                                        ; preds = %for.cond1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [10 x i32], [10 x i32]* %E, i64 %idxprom
  %idxprom4 = sext i32 %j.0 to i64
  %arrayidx5 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx, i64 0, i64 %idxprom4
  %0 = load i32, i32* %arrayidx5, align 4
  br label %for.cond6

for.cond6:                                        ; preds = %for.inc, %for.body3
  %k.0 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %tmp.0 = phi i32 [ %0, %for.body3 ], [ %add, %for.inc ]
  %cmp7 = icmp slt i32 %k.0, 10
  br i1 %cmp7, label %for.body8, label %for.end

for.body8:                                        ; preds = %for.cond6
  %idxprom9 = sext i32 %i.0 to i64
  %arrayidx10 = getelementptr inbounds [10 x i32], [10 x i32]* %A, i64 %idxprom9
  %idxprom11 = sext i32 %k.0 to i64
  %arrayidx12 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx10, i64 0, i64 %idxprom11
  %1 = load i32, i32* %arrayidx12, align 4
  %idxprom13 = sext i32 %k.0 to i64
  %arrayidx14 = getelementptr inbounds [10 x i32], [10 x i32]* %B, i64 %idxprom13
  %idxprom15 = sext i32 %j.0 to i64
  %arrayidx16 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx14, i64 0, i64 %idxprom15
  %2 = load i32, i32* %arrayidx16, align 4
  %mul = mul nsw i32 %1, %2
  %add = add nsw i32 %tmp.0, %mul
  br label %for.inc

for.inc:                                          ; preds = %for.body8
  %inc = add nsw i32 %k.0, 1
  br label %for.cond6

for.end:                                          ; preds = %for.cond6
  %idxprom17 = sext i32 %i.0 to i64
  %arrayidx18 = getelementptr inbounds [10 x i32], [10 x i32]* %E, i64 %idxprom17
  %idxprom19 = sext i32 %j.0 to i64
  %arrayidx20 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx18, i64 0, i64 %idxprom19
  store i32 %tmp.0, i32* %arrayidx20, align 4
  br label %for.inc21

for.inc21:                                        ; preds = %for.end
  %inc22 = add nsw i32 %j.0, 1
  br label %for.cond1

for.end23:                                        ; preds = %for.cond1
  br label %for.inc24

for.inc24:                                        ; preds = %for.end23
  %inc25 = add nsw i32 %i.0, 1
  br label %for.cond

for.end26:                                        ; preds = %for.cond
  br label %for.cond27

for.cond27:                                       ; preds = %for.inc61, %for.end26
  %i.1 = phi i32 [ 0, %for.end26 ], [ %inc62, %for.inc61 ]
  %cmp28 = icmp slt i32 %i.1, 10
  br i1 %cmp28, label %for.body29, label %for.end63

for.body29:                                       ; preds = %for.cond27
  br label %for.cond30

for.cond30:                                       ; preds = %for.inc58, %for.body29
  %j.1 = phi i32 [ 0, %for.body29 ], [ %inc59, %for.inc58 ]
  %cmp31 = icmp slt i32 %j.1, 10
  br i1 %cmp31, label %for.body32, label %for.end60

for.body32:                                       ; preds = %for.cond30
  %idxprom34 = sext i32 %i.1 to i64
  %arrayidx35 = getelementptr inbounds [10 x i32], [10 x i32]* %F, i64 %idxprom34
  %idxprom36 = sext i32 %j.1 to i64
  %arrayidx37 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx35, i64 0, i64 %idxprom36
  %3 = load i32, i32* %arrayidx37, align 4
  br label %for.cond38

for.cond38:                                       ; preds = %for.inc51, %for.body32
  %k.1 = phi i32 [ 0, %for.body32 ], [ %inc52, %for.inc51 ]
  %tmp33.0 = phi i32 [ %3, %for.body32 ], [ %add50, %for.inc51 ]
  %cmp39 = icmp slt i32 %k.1, 10
  br i1 %cmp39, label %for.body40, label %for.end53

for.body40:                                       ; preds = %for.cond38
  %idxprom41 = sext i32 %i.1 to i64
  %arrayidx42 = getelementptr inbounds [10 x i32], [10 x i32]* %C, i64 %idxprom41
  %idxprom43 = sext i32 %k.1 to i64
  %arrayidx44 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx42, i64 0, i64 %idxprom43
  %4 = load i32, i32* %arrayidx44, align 4
  %idxprom45 = sext i32 %k.1 to i64
  %arrayidx46 = getelementptr inbounds [10 x i32], [10 x i32]* %D, i64 %idxprom45
  %idxprom47 = sext i32 %j.1 to i64
  %arrayidx48 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx46, i64 0, i64 %idxprom47
  %5 = load i32, i32* %arrayidx48, align 4
  %mul49 = mul nsw i32 %4, %5
  %add50 = add nsw i32 %tmp33.0, %mul49
  br label %for.inc51

for.inc51:                                        ; preds = %for.body40
  %inc52 = add nsw i32 %k.1, 1
  br label %for.cond38

for.end53:                                        ; preds = %for.cond38
  %idxprom54 = sext i32 %i.1 to i64
  %arrayidx55 = getelementptr inbounds [10 x i32], [10 x i32]* %F, i64 %idxprom54
  %idxprom56 = sext i32 %j.1 to i64
  %arrayidx57 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx55, i64 0, i64 %idxprom56
  store i32 %tmp33.0, i32* %arrayidx57, align 4
  br label %for.inc58

for.inc58:                                        ; preds = %for.end53
  %inc59 = add nsw i32 %j.1, 1
  br label %for.cond30

for.end60:                                        ; preds = %for.cond30
  br label %for.inc61

for.inc61:                                        ; preds = %for.end60
  %inc62 = add nsw i32 %i.1, 1
  br label %for.cond27

for.end63:                                        ; preds = %for.cond27
  br label %for.cond64

for.cond64:                                       ; preds = %for.inc98, %for.end63
  %i.2 = phi i32 [ 0, %for.end63 ], [ %inc99, %for.inc98 ]
  %cmp65 = icmp slt i32 %i.2, 10
  br i1 %cmp65, label %for.body66, label %for.end100

for.body66:                                       ; preds = %for.cond64
  br label %for.cond67

for.cond67:                                       ; preds = %for.inc95, %for.body66
  %j.2 = phi i32 [ 0, %for.body66 ], [ %inc96, %for.inc95 ]
  %cmp68 = icmp slt i32 %j.2, 10
  br i1 %cmp68, label %for.body69, label %for.end97

for.body69:                                       ; preds = %for.cond67
  %idxprom71 = sext i32 %i.2 to i64
  %arrayidx72 = getelementptr inbounds [10 x i32], [10 x i32]* %G, i64 %idxprom71
  %idxprom73 = sext i32 %j.2 to i64
  %arrayidx74 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx72, i64 0, i64 %idxprom73
  %6 = load i32, i32* %arrayidx74, align 4
  br label %for.cond75

for.cond75:                                       ; preds = %for.inc88, %for.body69
  %k.2 = phi i32 [ 0, %for.body69 ], [ %inc89, %for.inc88 ]
  %tmp70.0 = phi i32 [ %6, %for.body69 ], [ %add87, %for.inc88 ]
  %cmp76 = icmp slt i32 %k.2, 10
  br i1 %cmp76, label %for.body77, label %for.end90

for.body77:                                       ; preds = %for.cond75
  %idxprom78 = sext i32 %i.2 to i64
  %arrayidx79 = getelementptr inbounds [10 x i32], [10 x i32]* %E, i64 %idxprom78
  %idxprom80 = sext i32 %k.2 to i64
  %arrayidx81 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx79, i64 0, i64 %idxprom80
  %7 = load i32, i32* %arrayidx81, align 4
  %idxprom82 = sext i32 %k.2 to i64
  %arrayidx83 = getelementptr inbounds [10 x i32], [10 x i32]* %F, i64 %idxprom82
  %idxprom84 = sext i32 %j.2 to i64
  %arrayidx85 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx83, i64 0, i64 %idxprom84
  %8 = load i32, i32* %arrayidx85, align 4
  %mul86 = mul nsw i32 %7, %8
  %add87 = add nsw i32 %tmp70.0, %mul86
  br label %for.inc88

for.inc88:                                        ; preds = %for.body77
  %inc89 = add nsw i32 %k.2, 1
  br label %for.cond75

for.end90:                                        ; preds = %for.cond75
  %idxprom91 = sext i32 %i.2 to i64
  %arrayidx92 = getelementptr inbounds [10 x i32], [10 x i32]* %G, i64 %idxprom91
  %idxprom93 = sext i32 %j.2 to i64
  %arrayidx94 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx92, i64 0, i64 %idxprom93
  store i32 %tmp70.0, i32* %arrayidx94, align 4
  br label %for.inc95

for.inc95:                                        ; preds = %for.end90
  %inc96 = add nsw i32 %j.2, 1
  br label %for.cond67

for.end97:                                        ; preds = %for.cond67
  br label %for.inc98

for.inc98:                                        ; preds = %for.end97
  %inc99 = add nsw i32 %i.2, 1
  br label %for.cond64

for.end100:                                       ; preds = %for.cond64
  ret i32 %i.2
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %A = alloca [1 x [10 x [10 x i32]]], align 16
  %B = alloca [1 x [10 x [10 x i32]]], align 16
  %C = alloca [1 x [10 x [10 x i32]]], align 16
  %D = alloca [1 x [10 x [10 x i32]]], align 16
  %E = alloca [1 x [10 x [10 x i32]]], align 16
  %F = alloca [1 x [10 x [10 x i32]]], align 16
  %G = alloca [1 x [10 x [10 x i32]]], align 16
  br label %for.cond

for.cond:                                         ; preds = %for.inc62, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc63, %for.inc62 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end64

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc59, %for.body
  %y.0 = phi i32 [ 0, %for.body ], [ %inc60, %for.inc59 ]
  %cmp2 = icmp slt i32 %y.0, 10
  br i1 %cmp2, label %for.body3, label %for.end61

for.body3:                                        ; preds = %for.cond1
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %x.0 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %cmp5 = icmp slt i32 %x.0, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 10
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %A, i64 0, i64 %idxprom
  %idxprom7 = sext i32 %y.0 to i64
  %arrayidx8 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx, i64 0, i64 %idxprom7
  %idxprom9 = sext i32 %x.0 to i64
  %arrayidx10 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx8, i64 0, i64 %idxprom9
  store i32 %rem, i32* %arrayidx10, align 4
  %call11 = call i32 @rand() #3
  %rem12 = srem i32 %call11, 10
  %idxprom13 = sext i32 %i.0 to i64
  %arrayidx14 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %B, i64 0, i64 %idxprom13
  %idxprom15 = sext i32 %y.0 to i64
  %arrayidx16 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx14, i64 0, i64 %idxprom15
  %idxprom17 = sext i32 %x.0 to i64
  %arrayidx18 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx16, i64 0, i64 %idxprom17
  store i32 %rem12, i32* %arrayidx18, align 4
  %call19 = call i32 @rand() #3
  %rem20 = srem i32 %call19, 10
  %idxprom21 = sext i32 %i.0 to i64
  %arrayidx22 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %C, i64 0, i64 %idxprom21
  %idxprom23 = sext i32 %y.0 to i64
  %arrayidx24 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx22, i64 0, i64 %idxprom23
  %idxprom25 = sext i32 %x.0 to i64
  %arrayidx26 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx24, i64 0, i64 %idxprom25
  store i32 %rem20, i32* %arrayidx26, align 4
  %call27 = call i32 @rand() #3
  %rem28 = srem i32 %call27, 10
  %idxprom29 = sext i32 %i.0 to i64
  %arrayidx30 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %D, i64 0, i64 %idxprom29
  %idxprom31 = sext i32 %y.0 to i64
  %arrayidx32 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx30, i64 0, i64 %idxprom31
  %idxprom33 = sext i32 %x.0 to i64
  %arrayidx34 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx32, i64 0, i64 %idxprom33
  store i32 %rem28, i32* %arrayidx34, align 4
  %call35 = call i32 @rand() #3
  %rem36 = srem i32 %call35, 10
  %idxprom37 = sext i32 %i.0 to i64
  %arrayidx38 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %E, i64 0, i64 %idxprom37
  %idxprom39 = sext i32 %y.0 to i64
  %arrayidx40 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx38, i64 0, i64 %idxprom39
  %idxprom41 = sext i32 %x.0 to i64
  %arrayidx42 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx40, i64 0, i64 %idxprom41
  store i32 %rem36, i32* %arrayidx42, align 4
  %call43 = call i32 @rand() #3
  %rem44 = srem i32 %call43, 10
  %idxprom45 = sext i32 %i.0 to i64
  %arrayidx46 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %F, i64 0, i64 %idxprom45
  %idxprom47 = sext i32 %y.0 to i64
  %arrayidx48 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx46, i64 0, i64 %idxprom47
  %idxprom49 = sext i32 %x.0 to i64
  %arrayidx50 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx48, i64 0, i64 %idxprom49
  store i32 %rem44, i32* %arrayidx50, align 4
  %call51 = call i32 @rand() #3
  %rem52 = srem i32 %call51, 10
  %idxprom53 = sext i32 %i.0 to i64
  %arrayidx54 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %G, i64 0, i64 %idxprom53
  %idxprom55 = sext i32 %y.0 to i64
  %arrayidx56 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx54, i64 0, i64 %idxprom55
  %idxprom57 = sext i32 %x.0 to i64
  %arrayidx58 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx56, i64 0, i64 %idxprom57
  store i32 %rem52, i32* %arrayidx58, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body6
  %inc = add nsw i32 %x.0, 1
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  br label %for.inc59

for.inc59:                                        ; preds = %for.end
  %inc60 = add nsw i32 %y.0, 1
  br label %for.cond1

for.end61:                                        ; preds = %for.cond1
  br label %for.inc62

for.inc62:                                        ; preds = %for.end61
  %inc63 = add nsw i32 %i.0, 1
  br label %for.cond

for.end64:                                        ; preds = %for.cond
  %arrayidx65 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %A, i64 0, i64 0
  %arraydecay = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx65, i32 0, i32 0
  %arrayidx66 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %B, i64 0, i64 0
  %arraydecay67 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx66, i32 0, i32 0
  %arrayidx68 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %C, i64 0, i64 0
  %arraydecay69 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx68, i32 0, i32 0
  %arrayidx70 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %D, i64 0, i64 0
  %arraydecay71 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx70, i32 0, i32 0
  %arrayidx72 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %E, i64 0, i64 0
  %arraydecay73 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx72, i32 0, i32 0
  %arrayidx74 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %F, i64 0, i64 0
  %arraydecay75 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx74, i32 0, i32 0
  %arrayidx76 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %G, i64 0, i64 0
  %arraydecay77 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx76, i32 0, i32 0
  %call78 = call i32 @_Z10kernel_3mmPA10_iS0_S0_S0_S0_S0_S0_([10 x i32]* %arraydecay, [10 x i32]* %arraydecay67, [10 x i32]* %arraydecay69, [10 x i32]* %arraydecay71, [10 x i32]* %arraydecay73, [10 x i32]* %arraydecay75, [10 x i32]* %arraydecay77)
  ret i32 0
}

; Function Attrs: nounwind
declare i32 @rand() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
