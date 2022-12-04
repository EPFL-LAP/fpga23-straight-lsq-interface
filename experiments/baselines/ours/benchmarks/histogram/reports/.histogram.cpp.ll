; ModuleID = 'src/histogram.cpp'
source_filename = "src/histogram.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z9histogramPiS_S_i(i32* %feature, i32* %weight, i32* %hist, i32 %n) #0 {
entry:
  %feature.addr = alloca i32*, align 8
  %weight.addr = alloca i32*, align 8
  %hist.addr = alloca i32*, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %m = alloca i32, align 4
  %wt = alloca i32, align 4
  %x = alloca i32, align 4
  store i32* %feature, i32** %feature.addr, align 8
  store i32* %weight, i32** %weight.addr, align 8
  store i32* %hist, i32** %hist.addr, align 8
  store i32 %n, i32* %n.addr, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32*, i32** %feature.addr, align 8
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds i32, i32* %2, i64 %idxprom
  %4 = load i32, i32* %arrayidx, align 4
  store i32 %4, i32* %m, align 4
  %5 = load i32*, i32** %weight.addr, align 8
  %6 = load i32, i32* %i, align 4
  %idxprom1 = sext i32 %6 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %5, i64 %idxprom1
  %7 = load i32, i32* %arrayidx2, align 4
  store i32 %7, i32* %wt, align 4
  %8 = load i32*, i32** %hist.addr, align 8
  %9 = load i32, i32* %m, align 4
  %idxprom3 = sext i32 %9 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %8, i64 %idxprom3
  %10 = load i32, i32* %arrayidx4, align 4
  store i32 %10, i32* %x, align 4
  %11 = load i32, i32* %x, align 4
  %12 = load i32, i32* %wt, align 4
  %add = add nsw i32 %11, %12
  %13 = load i32*, i32** %hist.addr, align 8
  %14 = load i32, i32* %m, align 4
  %idxprom5 = sext i32 %14 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %13, i64 %idxprom5
  store i32 %add, i32* %arrayidx6, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %15 = load i32, i32* %i, align 4
  %inc = add nsw i32 %15, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %16 = load i32, i32* %i, align 4
  ret i32 %16
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %feature = alloca [1 x [1000 x i32]], align 16
  %weight = alloca [1 x [1000 x i32]], align 16
  %hist = alloca [1 x [1000 x i32]], align 16
  %n = alloca [1 x i32], align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i23 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @srand(i32 13) #3
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc20, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end22

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %i, align 4
  %idxprom = sext i32 %1 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom
  store i32 1000, i32* %arrayidx, align 4
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %2, 1000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 1000
  %3 = load i32, i32* %i, align 4
  %idxprom4 = sext i32 %3 to i64
  %arrayidx5 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %feature, i64 0, i64 %idxprom4
  %4 = load i32, i32* %j, align 4
  %idxprom6 = sext i32 %4 to i64
  %arrayidx7 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  store i32 %rem, i32* %arrayidx7, align 4
  %call8 = call i32 @rand() #3
  %rem9 = srem i32 %call8, 100
  %5 = load i32, i32* %i, align 4
  %idxprom10 = sext i32 %5 to i64
  %arrayidx11 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %weight, i64 0, i64 %idxprom10
  %6 = load i32, i32* %j, align 4
  %idxprom12 = sext i32 %6 to i64
  %arrayidx13 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 %rem9, i32* %arrayidx13, align 4
  %call14 = call i32 @rand() #3
  %rem15 = srem i32 %call14, 100
  %7 = load i32, i32* %i, align 4
  %idxprom16 = sext i32 %7 to i64
  %arrayidx17 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %hist, i64 0, i64 %idxprom16
  %8 = load i32, i32* %j, align 4
  %idxprom18 = sext i32 %8 to i64
  %arrayidx19 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx17, i64 0, i64 %idxprom18
  store i32 %rem15, i32* %arrayidx19, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %9 = load i32, i32* %j, align 4
  %inc = add nsw i32 %9, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc20

for.inc20:                                        ; preds = %for.end
  %10 = load i32, i32* %i, align 4
  %inc21 = add nsw i32 %10, 1
  store i32 %inc21, i32* %i, align 4
  br label %for.cond

for.end22:                                        ; preds = %for.cond
  store i32 0, i32* %i23, align 4
  %11 = load i32, i32* %i23, align 4
  %idxprom24 = sext i32 %11 to i64
  %arrayidx25 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %feature, i64 0, i64 %idxprom24
  %arraydecay = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx25, i32 0, i32 0
  %12 = load i32, i32* %i23, align 4
  %idxprom26 = sext i32 %12 to i64
  %arrayidx27 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %weight, i64 0, i64 %idxprom26
  %arraydecay28 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx27, i32 0, i32 0
  %13 = load i32, i32* %i23, align 4
  %idxprom29 = sext i32 %13 to i64
  %arrayidx30 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %hist, i64 0, i64 %idxprom29
  %arraydecay31 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx30, i32 0, i32 0
  %14 = load i32, i32* %i23, align 4
  %idxprom32 = sext i32 %14 to i64
  %arrayidx33 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom32
  %15 = load i32, i32* %arrayidx33, align 4
  %call34 = call i32 @_Z9histogramPiS_S_i(i32* %arraydecay, i32* %arraydecay28, i32* %arraydecay31, i32 %15)
  %16 = load i32, i32* %retval, align 4
  ret i32 %16
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
