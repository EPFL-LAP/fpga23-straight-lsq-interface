; ModuleID = 'src/kernel_2mm.cpp'
source_filename = "src/kernel_2mm.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_(i32 %alpha, i32 %beta, [10 x i32]* %tmp, [10 x i32]* %A, [10 x i32]* %B, [10 x i32]* %C, [10 x i32]* %D) #0 {
entry:
  %alpha.addr = alloca i32, align 4
  %beta.addr = alloca i32, align 4
  %tmp.addr = alloca [10 x i32]*, align 8
  %A.addr = alloca [10 x i32]*, align 8
  %B.addr = alloca [10 x i32]*, align 8
  %C.addr = alloca [10 x i32]*, align 8
  %D.addr = alloca [10 x i32]*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %x = alloca i32, align 4
  %x34 = alloca i32, align 4
  store i32 %alpha, i32* %alpha.addr, align 4
  store i32 %beta, i32* %beta.addr, align 4
  store [10 x i32]* %tmp, [10 x i32]** %tmp.addr, align 8
  store [10 x i32]* %A, [10 x i32]** %A.addr, align 8
  store [10 x i32]* %B, [10 x i32]** %B.addr, align 8
  store [10 x i32]* %C, [10 x i32]** %C.addr, align 8
  store [10 x i32]* %D, [10 x i32]** %D.addr, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc25, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end27

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc22, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 10
  br i1 %cmp2, label %for.body3, label %for.end24

for.body3:                                        ; preds = %for.cond1
  %2 = load [10 x i32]*, [10 x i32]** %tmp.addr, align 8
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [10 x i32], [10 x i32]* %2, i64 %idxprom
  %4 = load i32, i32* %j, align 4
  %idxprom4 = sext i32 %4 to i64
  %arrayidx5 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx, i64 0, i64 %idxprom4
  %5 = load i32, i32* %arrayidx5, align 4
  store i32 %5, i32* %x, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond6

for.cond6:                                        ; preds = %for.inc, %for.body3
  %6 = load i32, i32* %k, align 4
  %cmp7 = icmp slt i32 %6, 10
  br i1 %cmp7, label %for.body8, label %for.end

for.body8:                                        ; preds = %for.cond6
  %7 = load i32, i32* %alpha.addr, align 4
  %8 = load [10 x i32]*, [10 x i32]** %A.addr, align 8
  %9 = load i32, i32* %i, align 4
  %idxprom9 = sext i32 %9 to i64
  %arrayidx10 = getelementptr inbounds [10 x i32], [10 x i32]* %8, i64 %idxprom9
  %10 = load i32, i32* %k, align 4
  %idxprom11 = sext i32 %10 to i64
  %arrayidx12 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx10, i64 0, i64 %idxprom11
  %11 = load i32, i32* %arrayidx12, align 4
  %mul = mul nsw i32 %7, %11
  %12 = load [10 x i32]*, [10 x i32]** %B.addr, align 8
  %13 = load i32, i32* %k, align 4
  %idxprom13 = sext i32 %13 to i64
  %arrayidx14 = getelementptr inbounds [10 x i32], [10 x i32]* %12, i64 %idxprom13
  %14 = load i32, i32* %j, align 4
  %idxprom15 = sext i32 %14 to i64
  %arrayidx16 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx14, i64 0, i64 %idxprom15
  %15 = load i32, i32* %arrayidx16, align 4
  %mul17 = mul nsw i32 %mul, %15
  %16 = load i32, i32* %x, align 4
  %add = add nsw i32 %16, %mul17
  store i32 %add, i32* %x, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body8
  %17 = load i32, i32* %k, align 4
  %inc = add nsw i32 %17, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond6

for.end:                                          ; preds = %for.cond6
  %18 = load i32, i32* %x, align 4
  %19 = load [10 x i32]*, [10 x i32]** %tmp.addr, align 8
  %20 = load i32, i32* %i, align 4
  %idxprom18 = sext i32 %20 to i64
  %arrayidx19 = getelementptr inbounds [10 x i32], [10 x i32]* %19, i64 %idxprom18
  %21 = load i32, i32* %j, align 4
  %idxprom20 = sext i32 %21 to i64
  %arrayidx21 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx19, i64 0, i64 %idxprom20
  store i32 %18, i32* %arrayidx21, align 4
  br label %for.inc22

for.inc22:                                        ; preds = %for.end
  %22 = load i32, i32* %j, align 4
  %inc23 = add nsw i32 %22, 1
  store i32 %inc23, i32* %j, align 4
  br label %for.cond1

for.end24:                                        ; preds = %for.cond1
  br label %for.inc25

for.inc25:                                        ; preds = %for.end24
  %23 = load i32, i32* %i, align 4
  %inc26 = add nsw i32 %23, 1
  store i32 %inc26, i32* %i, align 4
  br label %for.cond

for.end27:                                        ; preds = %for.cond
  store i32 0, i32* %i, align 4
  br label %for.cond28

for.cond28:                                       ; preds = %for.inc63, %for.end27
  %24 = load i32, i32* %i, align 4
  %cmp29 = icmp slt i32 %24, 10
  br i1 %cmp29, label %for.body30, label %for.end65

for.body30:                                       ; preds = %for.cond28
  store i32 0, i32* %j, align 4
  br label %for.cond31

for.cond31:                                       ; preds = %for.inc60, %for.body30
  %25 = load i32, i32* %j, align 4
  %cmp32 = icmp slt i32 %25, 10
  br i1 %cmp32, label %for.body33, label %for.end62

for.body33:                                       ; preds = %for.cond31
  %26 = load [10 x i32]*, [10 x i32]** %D.addr, align 8
  %27 = load i32, i32* %i, align 4
  %idxprom35 = sext i32 %27 to i64
  %arrayidx36 = getelementptr inbounds [10 x i32], [10 x i32]* %26, i64 %idxprom35
  %28 = load i32, i32* %j, align 4
  %idxprom37 = sext i32 %28 to i64
  %arrayidx38 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx36, i64 0, i64 %idxprom37
  %29 = load i32, i32* %arrayidx38, align 4
  %30 = load i32, i32* %beta.addr, align 4
  %mul39 = mul nsw i32 %29, %30
  store i32 %mul39, i32* %x34, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond40

for.cond40:                                       ; preds = %for.inc53, %for.body33
  %31 = load i32, i32* %k, align 4
  %cmp41 = icmp slt i32 %31, 10
  br i1 %cmp41, label %for.body42, label %for.end55

for.body42:                                       ; preds = %for.cond40
  %32 = load [10 x i32]*, [10 x i32]** %tmp.addr, align 8
  %33 = load i32, i32* %i, align 4
  %idxprom43 = sext i32 %33 to i64
  %arrayidx44 = getelementptr inbounds [10 x i32], [10 x i32]* %32, i64 %idxprom43
  %34 = load i32, i32* %k, align 4
  %idxprom45 = sext i32 %34 to i64
  %arrayidx46 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx44, i64 0, i64 %idxprom45
  %35 = load i32, i32* %arrayidx46, align 4
  %36 = load [10 x i32]*, [10 x i32]** %C.addr, align 8
  %37 = load i32, i32* %k, align 4
  %idxprom47 = sext i32 %37 to i64
  %arrayidx48 = getelementptr inbounds [10 x i32], [10 x i32]* %36, i64 %idxprom47
  %38 = load i32, i32* %j, align 4
  %idxprom49 = sext i32 %38 to i64
  %arrayidx50 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx48, i64 0, i64 %idxprom49
  %39 = load i32, i32* %arrayidx50, align 4
  %mul51 = mul nsw i32 %35, %39
  %40 = load i32, i32* %x34, align 4
  %add52 = add nsw i32 %40, %mul51
  store i32 %add52, i32* %x34, align 4
  br label %for.inc53

for.inc53:                                        ; preds = %for.body42
  %41 = load i32, i32* %k, align 4
  %inc54 = add nsw i32 %41, 1
  store i32 %inc54, i32* %k, align 4
  br label %for.cond40

for.end55:                                        ; preds = %for.cond40
  %42 = load i32, i32* %x34, align 4
  %43 = load [10 x i32]*, [10 x i32]** %D.addr, align 8
  %44 = load i32, i32* %i, align 4
  %idxprom56 = sext i32 %44 to i64
  %arrayidx57 = getelementptr inbounds [10 x i32], [10 x i32]* %43, i64 %idxprom56
  %45 = load i32, i32* %j, align 4
  %idxprom58 = sext i32 %45 to i64
  %arrayidx59 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx57, i64 0, i64 %idxprom58
  store i32 %42, i32* %arrayidx59, align 4
  br label %for.inc60

for.inc60:                                        ; preds = %for.end55
  %46 = load i32, i32* %j, align 4
  %inc61 = add nsw i32 %46, 1
  store i32 %inc61, i32* %j, align 4
  br label %for.cond31

for.end62:                                        ; preds = %for.cond31
  br label %for.inc63

for.inc63:                                        ; preds = %for.end62
  %47 = load i32, i32* %i, align 4
  %inc64 = add nsw i32 %47, 1
  store i32 %inc64, i32* %i, align 4
  br label %for.cond28

for.end65:                                        ; preds = %for.cond28
  %48 = load i32, i32* %k, align 4
  ret i32 %48
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %alpha = alloca [1 x i32], align 4
  %beta = alloca [1 x i32], align 4
  %tmp = alloca [1 x [10 x [10 x i32]]], align 16
  %A = alloca [1 x [10 x [10 x i32]]], align 16
  %B = alloca [1 x [10 x [10 x i32]]], align 16
  %C = alloca [1 x [10 x [10 x i32]]], align 16
  %D = alloca [1 x [10 x [10 x i32]]], align 16
  %i = alloca i32, align 4
  %y = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc44, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end46

for.body:                                         ; preds = %for.cond
  %call = call i32 @rand() #3
  %1 = load i32, i32* %i, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %alpha, i64 0, i64 %idxprom
  store i32 %call, i32* %arrayidx, align 4
  %call1 = call i32 @rand() #3
  %2 = load i32, i32* %i, align 4
  %idxprom2 = sext i32 %2 to i64
  %arrayidx3 = getelementptr inbounds [1 x i32], [1 x i32]* %beta, i64 0, i64 %idxprom2
  store i32 %call1, i32* %arrayidx3, align 4
  store i32 0, i32* %y, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc41, %for.body
  %3 = load i32, i32* %y, align 4
  %cmp5 = icmp slt i32 %3, 10
  br i1 %cmp5, label %for.body6, label %for.end43

for.body6:                                        ; preds = %for.cond4
  store i32 0, i32* %x, align 4
  br label %for.cond7

for.cond7:                                        ; preds = %for.inc, %for.body6
  %4 = load i32, i32* %x, align 4
  %cmp8 = icmp slt i32 %4, 10
  br i1 %cmp8, label %for.body9, label %for.end

for.body9:                                        ; preds = %for.cond7
  %call10 = call i32 @rand() #3
  %rem = srem i32 %call10, 100
  %5 = load i32, i32* %i, align 4
  %idxprom11 = sext i32 %5 to i64
  %arrayidx12 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %A, i64 0, i64 %idxprom11
  %6 = load i32, i32* %y, align 4
  %idxprom13 = sext i32 %6 to i64
  %arrayidx14 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx12, i64 0, i64 %idxprom13
  %7 = load i32, i32* %x, align 4
  %idxprom15 = sext i32 %7 to i64
  %arrayidx16 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx14, i64 0, i64 %idxprom15
  store i32 %rem, i32* %arrayidx16, align 4
  %call17 = call i32 @rand() #3
  %rem18 = srem i32 %call17, 100
  %8 = load i32, i32* %i, align 4
  %idxprom19 = sext i32 %8 to i64
  %arrayidx20 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %B, i64 0, i64 %idxprom19
  %9 = load i32, i32* %y, align 4
  %idxprom21 = sext i32 %9 to i64
  %arrayidx22 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx20, i64 0, i64 %idxprom21
  %10 = load i32, i32* %x, align 4
  %idxprom23 = sext i32 %10 to i64
  %arrayidx24 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx22, i64 0, i64 %idxprom23
  store i32 %rem18, i32* %arrayidx24, align 4
  %call25 = call i32 @rand() #3
  %rem26 = srem i32 %call25, 100
  %11 = load i32, i32* %i, align 4
  %idxprom27 = sext i32 %11 to i64
  %arrayidx28 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %C, i64 0, i64 %idxprom27
  %12 = load i32, i32* %y, align 4
  %idxprom29 = sext i32 %12 to i64
  %arrayidx30 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx28, i64 0, i64 %idxprom29
  %13 = load i32, i32* %x, align 4
  %idxprom31 = sext i32 %13 to i64
  %arrayidx32 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx30, i64 0, i64 %idxprom31
  store i32 %rem26, i32* %arrayidx32, align 4
  %call33 = call i32 @rand() #3
  %rem34 = srem i32 %call33, 100
  %14 = load i32, i32* %i, align 4
  %idxprom35 = sext i32 %14 to i64
  %arrayidx36 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %D, i64 0, i64 %idxprom35
  %15 = load i32, i32* %y, align 4
  %idxprom37 = sext i32 %15 to i64
  %arrayidx38 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx36, i64 0, i64 %idxprom37
  %16 = load i32, i32* %x, align 4
  %idxprom39 = sext i32 %16 to i64
  %arrayidx40 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx38, i64 0, i64 %idxprom39
  store i32 %rem34, i32* %arrayidx40, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body9
  %17 = load i32, i32* %x, align 4
  %inc = add nsw i32 %17, 1
  store i32 %inc, i32* %x, align 4
  br label %for.cond7

for.end:                                          ; preds = %for.cond7
  br label %for.inc41

for.inc41:                                        ; preds = %for.end
  %18 = load i32, i32* %y, align 4
  %inc42 = add nsw i32 %18, 1
  store i32 %inc42, i32* %y, align 4
  br label %for.cond4

for.end43:                                        ; preds = %for.cond4
  br label %for.inc44

for.inc44:                                        ; preds = %for.end43
  %19 = load i32, i32* %i, align 4
  %inc45 = add nsw i32 %19, 1
  store i32 %inc45, i32* %i, align 4
  br label %for.cond

for.end46:                                        ; preds = %for.cond
  %arrayidx47 = getelementptr inbounds [1 x i32], [1 x i32]* %alpha, i64 0, i64 0
  %20 = load i32, i32* %arrayidx47, align 4
  %arrayidx48 = getelementptr inbounds [1 x i32], [1 x i32]* %beta, i64 0, i64 0
  %21 = load i32, i32* %arrayidx48, align 4
  %arrayidx49 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %tmp, i64 0, i64 0
  %arraydecay = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx49, i32 0, i32 0
  %arrayidx50 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %A, i64 0, i64 0
  %arraydecay51 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx50, i32 0, i32 0
  %arrayidx52 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %B, i64 0, i64 0
  %arraydecay53 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx52, i32 0, i32 0
  %arrayidx54 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %C, i64 0, i64 0
  %arraydecay55 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx54, i32 0, i32 0
  %arrayidx56 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %D, i64 0, i64 0
  %arraydecay57 = getelementptr inbounds [10 x [10 x i32]], [10 x [10 x i32]]* %arrayidx56, i32 0, i32 0
  %call58 = call i32 @_Z10kernel_2mmiiPA10_iS0_S0_S0_S0_(i32 %20, i32 %21, [10 x i32]* %arraydecay, [10 x i32]* %arraydecay51, [10 x i32]* %arraydecay53, [10 x i32]* %arraydecay55, [10 x i32]* %arraydecay57)
  %22 = load i32, i32* %retval, align 4
  ret i32 %22
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
