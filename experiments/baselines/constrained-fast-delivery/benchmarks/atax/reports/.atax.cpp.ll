; ModuleID = 'src/atax.cpp'
source_filename = "src/atax.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z4ataxPA20_iPiS1_S1_([20 x i32]* %A, i32* %x, i32* %y, i32* %tmp) #0 {
entry:
  %A.addr = alloca [20 x i32]*, align 8
  %x.addr = alloca i32*, align 8
  %y.addr = alloca i32*, align 8
  %tmp.addr = alloca i32*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i32, align 4
  store [20 x i32]* %A, [20 x i32]** %A.addr, align 8
  store i32* %x, i32** %x.addr, align 8
  store i32* %y, i32** %y.addr, align 8
  store i32* %tmp, i32** %tmp.addr, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc28, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 20
  br i1 %cmp, label %for.body, label %for.end30

for.body:                                         ; preds = %for.cond
  %1 = load i32*, i32** %tmp.addr, align 8
  %2 = load i32, i32* %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds i32, i32* %1, i64 %idxprom
  %3 = load i32, i32* %arrayidx, align 4
  store i32 %3, i32* %t, align 4
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %4 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %4, 20
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %5 = load i32, i32* %t, align 4
  %6 = load [20 x i32]*, [20 x i32]** %A.addr, align 8
  %7 = load i32, i32* %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds [20 x i32], [20 x i32]* %6, i64 %idxprom4
  %8 = load i32, i32* %j, align 4
  %idxprom6 = sext i32 %8 to i64
  %arrayidx7 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  %9 = load i32, i32* %arrayidx7, align 4
  %10 = load i32*, i32** %x.addr, align 8
  %11 = load i32, i32* %j, align 4
  %idxprom8 = sext i32 %11 to i64
  %arrayidx9 = getelementptr inbounds i32, i32* %10, i64 %idxprom8
  %12 = load i32, i32* %arrayidx9, align 4
  %mul = mul nsw i32 %9, %12
  %add = add nsw i32 %5, %mul
  store i32 %add, i32* %t, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %13 = load i32, i32* %j, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  store i32 0, i32* %j, align 4
  br label %for.cond10

for.cond10:                                       ; preds = %for.inc23, %for.end
  %14 = load i32, i32* %j, align 4
  %cmp11 = icmp slt i32 %14, 20
  br i1 %cmp11, label %for.body12, label %for.end25

for.body12:                                       ; preds = %for.cond10
  %15 = load i32*, i32** %y.addr, align 8
  %16 = load i32, i32* %j, align 4
  %idxprom13 = sext i32 %16 to i64
  %arrayidx14 = getelementptr inbounds i32, i32* %15, i64 %idxprom13
  %17 = load i32, i32* %arrayidx14, align 4
  %18 = load [20 x i32]*, [20 x i32]** %A.addr, align 8
  %19 = load i32, i32* %i, align 4
  %idxprom15 = sext i32 %19 to i64
  %arrayidx16 = getelementptr inbounds [20 x i32], [20 x i32]* %18, i64 %idxprom15
  %20 = load i32, i32* %j, align 4
  %idxprom17 = sext i32 %20 to i64
  %arrayidx18 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx16, i64 0, i64 %idxprom17
  %21 = load i32, i32* %arrayidx18, align 4
  %22 = load i32, i32* %t, align 4
  %mul19 = mul nsw i32 %21, %22
  %add20 = add nsw i32 %17, %mul19
  %23 = load i32*, i32** %y.addr, align 8
  %24 = load i32, i32* %j, align 4
  %idxprom21 = sext i32 %24 to i64
  %arrayidx22 = getelementptr inbounds i32, i32* %23, i64 %idxprom21
  store i32 %add20, i32* %arrayidx22, align 4
  br label %for.inc23

for.inc23:                                        ; preds = %for.body12
  %25 = load i32, i32* %j, align 4
  %inc24 = add nsw i32 %25, 1
  store i32 %inc24, i32* %j, align 4
  br label %for.cond10

for.end25:                                        ; preds = %for.cond10
  %26 = load i32, i32* %t, align 4
  %27 = load i32*, i32** %tmp.addr, align 8
  %28 = load i32, i32* %i, align 4
  %idxprom26 = sext i32 %28 to i64
  %arrayidx27 = getelementptr inbounds i32, i32* %27, i64 %idxprom26
  store i32 %26, i32* %arrayidx27, align 4
  br label %for.inc28

for.inc28:                                        ; preds = %for.end25
  %29 = load i32, i32* %i, align 4
  %inc29 = add nsw i32 %29, 1
  store i32 %inc29, i32* %i, align 4
  br label %for.cond

for.end30:                                        ; preds = %for.cond
  %30 = load i32, i32* %i, align 4
  ret i32 %30
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %A = alloca [1 x [20 x [20 x i32]]], align 16
  %x = alloca [1 x [20 x i32]], align 16
  %y = alloca [1 x [20 x i32]], align 16
  %tmp = alloca [1 x [20 x i32]], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %x14 = alloca i32, align 4
  %i32 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @srand(i32 13) #3
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc29, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end31

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc26, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 20
  br i1 %cmp2, label %for.body3, label %for.end28

for.body3:                                        ; preds = %for.cond1
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %2 = load i32, i32* %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %x, i64 0, i64 %idxprom
  %3 = load i32, i32* %j, align 4
  %idxprom4 = sext i32 %3 to i64
  %arrayidx5 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %rem, i32* %arrayidx5, align 4
  %4 = load i32, i32* %i, align 4
  %idxprom6 = sext i32 %4 to i64
  %arrayidx7 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %y, i64 0, i64 %idxprom6
  %5 = load i32, i32* %j, align 4
  %idxprom8 = sext i32 %5 to i64
  %arrayidx9 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 0, i32* %arrayidx9, align 4
  %6 = load i32, i32* %i, align 4
  %idxprom10 = sext i32 %6 to i64
  %arrayidx11 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %tmp, i64 0, i64 %idxprom10
  %7 = load i32, i32* %j, align 4
  %idxprom12 = sext i32 %7 to i64
  %arrayidx13 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 0, i32* %arrayidx13, align 4
  store i32 0, i32* %x14, align 4
  br label %for.cond15

for.cond15:                                       ; preds = %for.inc, %for.body3
  %8 = load i32, i32* %x14, align 4
  %cmp16 = icmp slt i32 %8, 20
  br i1 %cmp16, label %for.body17, label %for.end

for.body17:                                       ; preds = %for.cond15
  %call18 = call i32 @rand() #3
  %rem19 = srem i32 %call18, 100
  %9 = load i32, i32* %i, align 4
  %idxprom20 = sext i32 %9 to i64
  %arrayidx21 = getelementptr inbounds [1 x [20 x [20 x i32]]], [1 x [20 x [20 x i32]]]* %A, i64 0, i64 %idxprom20
  %10 = load i32, i32* %j, align 4
  %idxprom22 = sext i32 %10 to i64
  %arrayidx23 = getelementptr inbounds [20 x [20 x i32]], [20 x [20 x i32]]* %arrayidx21, i64 0, i64 %idxprom22
  %11 = load i32, i32* %x14, align 4
  %idxprom24 = sext i32 %11 to i64
  %arrayidx25 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx23, i64 0, i64 %idxprom24
  store i32 %rem19, i32* %arrayidx25, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body17
  %12 = load i32, i32* %x14, align 4
  %inc = add nsw i32 %12, 1
  store i32 %inc, i32* %x14, align 4
  br label %for.cond15

for.end:                                          ; preds = %for.cond15
  br label %for.inc26

for.inc26:                                        ; preds = %for.end
  %13 = load i32, i32* %j, align 4
  %inc27 = add nsw i32 %13, 1
  store i32 %inc27, i32* %j, align 4
  br label %for.cond1

for.end28:                                        ; preds = %for.cond1
  br label %for.inc29

for.inc29:                                        ; preds = %for.end28
  %14 = load i32, i32* %i, align 4
  %inc30 = add nsw i32 %14, 1
  store i32 %inc30, i32* %i, align 4
  br label %for.cond

for.end31:                                        ; preds = %for.cond
  store i32 0, i32* %i32, align 4
  %arrayidx33 = getelementptr inbounds [1 x [20 x [20 x i32]]], [1 x [20 x [20 x i32]]]* %A, i64 0, i64 0
  %arraydecay = getelementptr inbounds [20 x [20 x i32]], [20 x [20 x i32]]* %arrayidx33, i32 0, i32 0
  %arrayidx34 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %x, i64 0, i64 0
  %arraydecay35 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx34, i32 0, i32 0
  %arrayidx36 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %y, i64 0, i64 0
  %arraydecay37 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx36, i32 0, i32 0
  %arrayidx38 = getelementptr inbounds [1 x [20 x i32]], [1 x [20 x i32]]* %tmp, i64 0, i64 0
  %arraydecay39 = getelementptr inbounds [20 x i32], [20 x i32]* %arrayidx38, i32 0, i32 0
  %call40 = call i32 @_Z4ataxPA20_iPiS1_S1_([20 x i32]* %arraydecay, i32* %arraydecay35, i32* %arraydecay37, i32* %arraydecay39)
  %15 = load i32, i32* %retval, align 4
  ret i32 %15
}

; Function Attrs: nounwind
declare void @srand(i32) #2

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
