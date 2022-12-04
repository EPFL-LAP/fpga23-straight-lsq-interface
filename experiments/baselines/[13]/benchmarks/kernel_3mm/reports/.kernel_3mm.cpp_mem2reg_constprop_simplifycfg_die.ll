; ModuleID = '.kernel_3mm.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/kernel_3mm.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z10kernel_3mmPA10_iS0_S0_S0_S0_S0_S0_([10 x i32]* %A, [10 x i32]* %B, [10 x i32]* %C, [10 x i32]* %D, [10 x i32]* %E, [10 x i32]* %F, [10 x i32]* %G) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc24, %entry
  %i.012 = phi i32 [ 0, %entry ], [ %inc25, %for.inc24 ]
  br label %for.body3

for.body3:                                        ; preds = %for.end, %for.body
  %j.011 = phi i32 [ 0, %for.body ], [ %inc22, %for.end ]
  %0 = zext i32 %i.012 to i64
  %1 = zext i32 %j.011 to i64
  %arrayidx5 = getelementptr inbounds [10 x i32], [10 x i32]* %E, i64 %0, i64 %1
  %2 = load i32, i32* %arrayidx5, align 4
  br label %for.body8

for.body8:                                        ; preds = %for.body8, %for.body3
  %tmp.010 = phi i32 [ %2, %for.body3 ], [ %add, %for.body8 ]
  %k.09 = phi i32 [ 0, %for.body3 ], [ %inc, %for.body8 ]
  %3 = zext i32 %i.012 to i64
  %4 = zext i32 %k.09 to i64
  %arrayidx12 = getelementptr inbounds [10 x i32], [10 x i32]* %A, i64 %3, i64 %4
  %5 = load i32, i32* %arrayidx12, align 4
  %6 = zext i32 %k.09 to i64
  %7 = zext i32 %j.011 to i64
  %arrayidx16 = getelementptr inbounds [10 x i32], [10 x i32]* %B, i64 %6, i64 %7
  %8 = load i32, i32* %arrayidx16, align 4
  %mul = mul nsw i32 %5, %8
  %add = add nsw i32 %tmp.010, %mul
  %inc = add nuw nsw i32 %k.09, 1
  %cmp7 = icmp ult i32 %inc, 10
  br i1 %cmp7, label %for.body8, label %for.end

for.end:                                          ; preds = %for.body8
  %9 = zext i32 %i.012 to i64
  %10 = zext i32 %j.011 to i64
  %arrayidx20 = getelementptr inbounds [10 x i32], [10 x i32]* %E, i64 %9, i64 %10
  store i32 %add, i32* %arrayidx20, align 4
  %inc22 = add nuw nsw i32 %j.011, 1
  %cmp2 = icmp ult i32 %inc22, 10
  br i1 %cmp2, label %for.body3, label %for.inc24

for.inc24:                                        ; preds = %for.end
  %inc25 = add nuw nsw i32 %i.012, 1
  %cmp = icmp ult i32 %inc25, 10
  br i1 %cmp, label %for.body, label %for.end26

for.end26:                                        ; preds = %for.inc24
  br label %for.body29

for.body29:                                       ; preds = %for.inc61, %for.end26
  %i.18 = phi i32 [ 0, %for.end26 ], [ %inc62, %for.inc61 ]
  br label %for.body32

for.body32:                                       ; preds = %for.end53, %for.body29
  %j.17 = phi i32 [ 0, %for.body29 ], [ %inc59, %for.end53 ]
  %11 = zext i32 %i.18 to i64
  %12 = zext i32 %j.17 to i64
  %arrayidx37 = getelementptr inbounds [10 x i32], [10 x i32]* %F, i64 %11, i64 %12
  %13 = load i32, i32* %arrayidx37, align 4
  br label %for.body40

for.body40:                                       ; preds = %for.body40, %for.body32
  %tmp33.06 = phi i32 [ %13, %for.body32 ], [ %add50, %for.body40 ]
  %k.15 = phi i32 [ 0, %for.body32 ], [ %inc52, %for.body40 ]
  %14 = zext i32 %i.18 to i64
  %15 = zext i32 %k.15 to i64
  %arrayidx44 = getelementptr inbounds [10 x i32], [10 x i32]* %C, i64 %14, i64 %15
  %16 = load i32, i32* %arrayidx44, align 4
  %17 = zext i32 %k.15 to i64
  %18 = zext i32 %j.17 to i64
  %arrayidx48 = getelementptr inbounds [10 x i32], [10 x i32]* %D, i64 %17, i64 %18
  %19 = load i32, i32* %arrayidx48, align 4
  %mul49 = mul nsw i32 %16, %19
  %add50 = add nsw i32 %tmp33.06, %mul49
  %inc52 = add nuw nsw i32 %k.15, 1
  %cmp39 = icmp ult i32 %inc52, 10
  br i1 %cmp39, label %for.body40, label %for.end53

for.end53:                                        ; preds = %for.body40
  %20 = zext i32 %i.18 to i64
  %21 = zext i32 %j.17 to i64
  %arrayidx57 = getelementptr inbounds [10 x i32], [10 x i32]* %F, i64 %20, i64 %21
  store i32 %add50, i32* %arrayidx57, align 4
  %inc59 = add nuw nsw i32 %j.17, 1
  %cmp31 = icmp ult i32 %inc59, 10
  br i1 %cmp31, label %for.body32, label %for.inc61

for.inc61:                                        ; preds = %for.end53
  %inc62 = add nuw nsw i32 %i.18, 1
  %cmp28 = icmp ult i32 %inc62, 10
  br i1 %cmp28, label %for.body29, label %for.end63

for.end63:                                        ; preds = %for.inc61
  br label %for.body66

for.body66:                                       ; preds = %for.inc98, %for.end63
  %i.24 = phi i32 [ 0, %for.end63 ], [ %inc99, %for.inc98 ]
  br label %for.body69

for.body69:                                       ; preds = %for.end90, %for.body66
  %j.23 = phi i32 [ 0, %for.body66 ], [ %inc96, %for.end90 ]
  %22 = zext i32 %i.24 to i64
  %23 = zext i32 %j.23 to i64
  %arrayidx74 = getelementptr inbounds [10 x i32], [10 x i32]* %G, i64 %22, i64 %23
  %24 = load i32, i32* %arrayidx74, align 4
  br label %for.body77

for.body77:                                       ; preds = %for.body77, %for.body69
  %tmp70.02 = phi i32 [ %24, %for.body69 ], [ %add87, %for.body77 ]
  %k.21 = phi i32 [ 0, %for.body69 ], [ %inc89, %for.body77 ]
  %25 = zext i32 %i.24 to i64
  %26 = zext i32 %k.21 to i64
  %arrayidx81 = getelementptr inbounds [10 x i32], [10 x i32]* %E, i64 %25, i64 %26
  %27 = load i32, i32* %arrayidx81, align 4
  %28 = zext i32 %k.21 to i64
  %29 = zext i32 %j.23 to i64
  %arrayidx85 = getelementptr inbounds [10 x i32], [10 x i32]* %F, i64 %28, i64 %29
  %30 = load i32, i32* %arrayidx85, align 4
  %mul86 = mul nsw i32 %27, %30
  %add87 = add nsw i32 %tmp70.02, %mul86
  %inc89 = add nuw nsw i32 %k.21, 1
  %cmp76 = icmp ult i32 %inc89, 10
  br i1 %cmp76, label %for.body77, label %for.end90

for.end90:                                        ; preds = %for.body77
  %31 = zext i32 %i.24 to i64
  %32 = zext i32 %j.23 to i64
  %arrayidx94 = getelementptr inbounds [10 x i32], [10 x i32]* %G, i64 %31, i64 %32
  store i32 %add87, i32* %arrayidx94, align 4
  %inc96 = add nuw nsw i32 %j.23, 1
  %cmp68 = icmp ult i32 %inc96, 10
  br i1 %cmp68, label %for.body69, label %for.inc98

for.inc98:                                        ; preds = %for.end90
  %inc99 = add nuw nsw i32 %i.24, 1
  %cmp65 = icmp ult i32 %inc99, 10
  br i1 %cmp65, label %for.body66, label %for.end100

for.end100:                                       ; preds = %for.inc98
  ret i32 %inc99
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
  br label %for.body

for.body:                                         ; preds = %for.inc62, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.inc59, %for.body
  %y.02 = phi i32 [ 0, %for.body ], [ %inc60, %for.inc59 ]
  br label %for.body6

for.body6:                                        ; preds = %for.body6, %for.body3
  %x.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.body6 ]
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 10
  %0 = zext i32 %y.02 to i64
  %1 = zext i32 %x.01 to i64
  %2 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %A, i64 0, i64 0, i64 %0, i64 %1
  store i32 %rem, i32* %2, align 4
  %call11 = call i32 @rand() #3
  %rem12 = srem i32 %call11, 10
  %3 = zext i32 %y.02 to i64
  %4 = zext i32 %x.01 to i64
  %5 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %B, i64 0, i64 0, i64 %3, i64 %4
  store i32 %rem12, i32* %5, align 4
  %call19 = call i32 @rand() #3
  %rem20 = srem i32 %call19, 10
  %6 = zext i32 %y.02 to i64
  %7 = zext i32 %x.01 to i64
  %8 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %C, i64 0, i64 0, i64 %6, i64 %7
  store i32 %rem20, i32* %8, align 4
  %call27 = call i32 @rand() #3
  %rem28 = srem i32 %call27, 10
  %9 = zext i32 %y.02 to i64
  %10 = zext i32 %x.01 to i64
  %11 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %D, i64 0, i64 0, i64 %9, i64 %10
  store i32 %rem28, i32* %11, align 4
  %call35 = call i32 @rand() #3
  %rem36 = srem i32 %call35, 10
  %12 = zext i32 %y.02 to i64
  %13 = zext i32 %x.01 to i64
  %14 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %E, i64 0, i64 0, i64 %12, i64 %13
  store i32 %rem36, i32* %14, align 4
  %call43 = call i32 @rand() #3
  %rem44 = srem i32 %call43, 10
  %15 = zext i32 %y.02 to i64
  %16 = zext i32 %x.01 to i64
  %17 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %F, i64 0, i64 0, i64 %15, i64 %16
  store i32 %rem44, i32* %17, align 4
  %call51 = call i32 @rand() #3
  %rem52 = srem i32 %call51, 10
  %18 = zext i32 %y.02 to i64
  %19 = zext i32 %x.01 to i64
  %20 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %G, i64 0, i64 0, i64 %18, i64 %19
  store i32 %rem52, i32* %20, align 4
  %inc = add nuw nsw i32 %x.01, 1
  %cmp5 = icmp ult i32 %inc, 10
  br i1 %cmp5, label %for.body6, label %for.inc59

for.inc59:                                        ; preds = %for.body6
  %inc60 = add nuw nsw i32 %y.02, 1
  %cmp2 = icmp ult i32 %inc60, 10
  br i1 %cmp2, label %for.body3, label %for.inc62

for.inc62:                                        ; preds = %for.inc59
  br i1 false, label %for.body, label %for.end64

for.end64:                                        ; preds = %for.inc62
  %arraydecay = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %A, i64 0, i64 0, i64 0
  %arraydecay67 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %B, i64 0, i64 0, i64 0
  %arraydecay69 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %C, i64 0, i64 0, i64 0
  %arraydecay71 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %D, i64 0, i64 0, i64 0
  %arraydecay73 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %E, i64 0, i64 0, i64 0
  %arraydecay75 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %F, i64 0, i64 0, i64 0
  %arraydecay77 = getelementptr inbounds [1 x [10 x [10 x i32]]], [1 x [10 x [10 x i32]]]* %G, i64 0, i64 0, i64 0
  %call78 = call i32 @_Z10kernel_3mmPA10_iS0_S0_S0_S0_S0_S0_([10 x i32]* nonnull %arraydecay, [10 x i32]* nonnull %arraydecay67, [10 x i32]* nonnull %arraydecay69, [10 x i32]* nonnull %arraydecay71, [10 x i32]* nonnull %arraydecay73, [10 x i32]* nonnull %arraydecay75, [10 x i32]* nonnull %arraydecay77)
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
