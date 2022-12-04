; ModuleID = 'src/triangular.cpp'
source_filename = "src/triangular.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10triangularPiPA100_ii(i32* %x, [100 x i32]* %A, i32 %n) #0 {
entry:
  %x.addr = alloca i32*, align 8
  %A.addr = alloca [100 x i32]*, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %k = alloca i32, align 4
  store i32* %x, i32** %x.addr, align 8
  store [100 x i32]* %A, [100 x i32]** %A.addr, align 8
  store i32 %n, i32* %n.addr, align 4
  %0 = load i32, i32* %n.addr, align 4
  %sub = sub nsw i32 %0, 1
  store i32 %sub, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc14, %entry
  %1 = load i32, i32* %i, align 4
  %cmp = icmp sge i32 %1, 0
  br i1 %cmp, label %for.body, label %for.end16

for.body:                                         ; preds = %for.cond
  %2 = load i32, i32* %i, align 4
  %sub1 = sub nsw i32 %2, 1
  store i32 %sub1, i32* %k, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc, %for.body
  %3 = load i32, i32* %k, align 4
  %cmp3 = icmp sge i32 %3, 0
  br i1 %cmp3, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond2
  %4 = load [100 x i32]*, [100 x i32]** %A.addr, align 8
  %5 = load i32, i32* %k, align 4
  %idxprom = sext i32 %5 to i64
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* %4, i64 %idxprom
  %6 = load i32, i32* %i, align 4
  %idxprom5 = sext i32 %6 to i64
  %arrayidx6 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx, i64 0, i64 %idxprom5
  %7 = load i32, i32* %arrayidx6, align 4
  %8 = load i32*, i32** %x.addr, align 8
  %9 = load i32, i32* %i, align 4
  %idxprom7 = sext i32 %9 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %8, i64 %idxprom7
  %10 = load i32, i32* %arrayidx8, align 4
  %mul = mul nsw i32 %7, %10
  %11 = load [100 x i32]*, [100 x i32]** %A.addr, align 8
  %12 = load i32, i32* %k, align 4
  %idxprom9 = sext i32 %12 to i64
  %arrayidx10 = getelementptr inbounds [100 x i32], [100 x i32]* %11, i64 %idxprom9
  %13 = load i32, i32* %n.addr, align 4
  %idxprom11 = sext i32 %13 to i64
  %arrayidx12 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx10, i64 0, i64 %idxprom11
  %14 = load i32, i32* %arrayidx12, align 4
  %sub13 = sub nsw i32 %14, %mul
  store i32 %sub13, i32* %arrayidx12, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %15 = load i32, i32* %k, align 4
  %dec = add nsw i32 %15, -1
  store i32 %dec, i32* %k, align 4
  br label %for.cond2

for.end:                                          ; preds = %for.cond2
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %16 = load i32, i32* %i, align 4
  %dec15 = add nsw i32 %16, -1
  store i32 %dec15, i32* %i, align 4
  br label %for.cond

for.end16:                                        ; preds = %for.cond
  %17 = load i32, i32* %i, align 4
  %18 = load i32, i32* %n.addr, align 4
  %add = add nsw i32 %17, %18
  ret i32 %add
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %xArray = alloca [1 x [100 x i32]], align 16
  %A = alloca [1 x [100 x [100 x i32]]], align 16
  %n = alloca [1 x i32], align 4
  %i = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %i25 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc22, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end24

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %i, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom
  store i32 100, i32* %arrayidx, align 4
  store i32 0, i32* %x, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc19, %for.body
  %2 = load i32, i32* %x, align 4
  %cmp2 = icmp slt i32 %2, 100
  br i1 %cmp2, label %for.body3, label %for.end21

for.body3:                                        ; preds = %for.cond1
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 1000
  %3 = load i32, i32* %i, align 4
  %idxprom4 = sext i32 %3 to i64
  %arrayidx5 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %xArray, i64 0, i64 %idxprom4
  %4 = load i32, i32* %x, align 4
  %idxprom6 = sext i32 %4 to i64
  %arrayidx7 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  store i32 %rem, i32* %arrayidx7, align 4
  store i32 0, i32* %y, align 4
  br label %for.cond8

for.cond8:                                        ; preds = %for.inc, %for.body3
  %5 = load i32, i32* %y, align 4
  %cmp9 = icmp slt i32 %5, 10
  br i1 %cmp9, label %for.body10, label %for.end

for.body10:                                       ; preds = %for.cond8
  %call11 = call i32 @rand() #3
  %rem12 = srem i32 %call11, 1000
  %6 = load i32, i32* %i, align 4
  %idxprom13 = sext i32 %6 to i64
  %arrayidx14 = getelementptr inbounds [1 x [100 x [100 x i32]]], [1 x [100 x [100 x i32]]]* %A, i64 0, i64 %idxprom13
  %7 = load i32, i32* %y, align 4
  %idxprom15 = sext i32 %7 to i64
  %arrayidx16 = getelementptr inbounds [100 x [100 x i32]], [100 x [100 x i32]]* %arrayidx14, i64 0, i64 %idxprom15
  %8 = load i32, i32* %x, align 4
  %idxprom17 = sext i32 %8 to i64
  %arrayidx18 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx16, i64 0, i64 %idxprom17
  store i32 %rem12, i32* %arrayidx18, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body10
  %9 = load i32, i32* %y, align 4
  %inc = add nsw i32 %9, 1
  store i32 %inc, i32* %y, align 4
  br label %for.cond8

for.end:                                          ; preds = %for.cond8
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %10 = load i32, i32* %x, align 4
  %inc20 = add nsw i32 %10, 1
  store i32 %inc20, i32* %x, align 4
  br label %for.cond1

for.end21:                                        ; preds = %for.cond1
  br label %for.inc22

for.inc22:                                        ; preds = %for.end21
  %11 = load i32, i32* %i, align 4
  %inc23 = add nsw i32 %11, 1
  store i32 %inc23, i32* %i, align 4
  br label %for.cond

for.end24:                                        ; preds = %for.cond
  store i32 0, i32* %i25, align 4
  %12 = load i32, i32* %i25, align 4
  %idxprom26 = sext i32 %12 to i64
  %arrayidx27 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %xArray, i64 0, i64 %idxprom26
  %arraydecay = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx27, i32 0, i32 0
  %13 = load i32, i32* %i25, align 4
  %idxprom28 = sext i32 %13 to i64
  %arrayidx29 = getelementptr inbounds [1 x [100 x [100 x i32]]], [1 x [100 x [100 x i32]]]* %A, i64 0, i64 %idxprom28
  %arraydecay30 = getelementptr inbounds [100 x [100 x i32]], [100 x [100 x i32]]* %arrayidx29, i32 0, i32 0
  %14 = load i32, i32* %i25, align 4
  %idxprom31 = sext i32 %14 to i64
  %arrayidx32 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom31
  %15 = load i32, i32* %arrayidx32, align 4
  %call33 = call i32 @_Z10triangularPiPA100_ii(i32* %arraydecay, [100 x i32]* %arraydecay30, i32 %15)
  %16 = load i32, i32* %retval, align 4
  ret i32 %16
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
